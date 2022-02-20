#include "RpcChannel.h"
#include "Logger.h"

#include "RpcController.h"

#include "google/protobuf/descriptor.h"
#include "boost/asio.hpp"

#include "rpc_meta.pb.h"

using namespace ecron::net;

static const std::unordered_map<int, std::string> _CodeError = {
    //{ 0, "ok" },
    {1001, "no service"},
    {1002, "no method"},
    {1003, "invalid request"},
    {1004, "invalid response"},
    {1005, "internal error"},
};

// 12-byte header [ERPC][body_size][meta_size]
static const size_t _HeaderLen = 12;

typedef std::shared_ptr<google::protobuf::Message> MessagePtr;


RpcChannel::RpcChannel() {
}

RpcChannel::~RpcChannel() {
    // LOG_TRACE("RpcChannel destructing");
    for (const auto& item : reqs_) {
        RpcController* ctrl = item.second;
        delete ctrl->get_resp();
        delete ctrl->get_done();
        delete ctrl;
    }
}

void RpcChannel::set_conn(const TcpConnectionPtr& conn) {
    conn_weak_ = conn;
}

void RpcChannel::set_services(std::unordered_map<std::string, google::protobuf::Service*>* services) {
    services_ = services;
}

// client send request(prepare response object)
void RpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                            google::protobuf::RpcController* controller,
                            const google::protobuf::Message* request,
                            google::protobuf::Message* response,
                            google::protobuf::Closure* done) {
    // packed request
    RpcMeta meta;
    int64_t id = id_.fetch_add(1);
    meta.set_correlation_id(id);
    RpcRequestMeta* request_meta = meta.mutable_request();
    request_meta->set_service_name(method->service()->full_name());
    request_meta->set_method_name(method->name());
    // message.set_request(request->SerializeAsString());

    // save request associated info
    RpcController* ctrl = static_cast<RpcController*>(controller);
    ctrl->set_resp(response);
    ctrl->set_done(done);
    reqs_[id] = ctrl;

    // send request
    pack_send(&meta, request);
}

void RpcChannel::process(Buffer* buffer, size_t len) {
    // process rpc protocol
    while (buffer->readable_bytes() >= _HeaderLen) {
        // ================= parse header ====================
        // char header[_HeaderLen] = { 0 };
        char* header = const_cast<char*>(buffer->begin_read());
        uint32_t body_len = 0;
        uint32_t meta_len = 0;
        // memcpy(header, buffer->begin_read(), _HeaderLen);

        if (0 != memcmp(header, "ERPC", 4)) { // check identifier
            LOG_ERROR("header identifier error");
            return;
        }

        int32_t* temp = reinterpret_cast<int32_t*>(header + 4);
        body_len = boost::asio::detail::socket_ops::network_to_host_long(*temp);
        temp = reinterpret_cast<int32_t*>(header + 8);
        meta_len = boost::asio::detail::socket_ops::network_to_host_long(*temp);

        // if (body_len > max_length) { // check body length of body and meta
        //     return;
        // }
        if (body_len < meta_len) {
            LOG_ERROR("header len error: {} {}", body_len, meta_len);
            return;
        }

        if (buffer->readable_bytes() < _HeaderLen + body_len) { // not complete message
            return;
        }

        // ================= parse meta ====================
        ErrorCode errorCode = ENOERROR;
        RpcMeta meta;
        if (!meta.ParseFromArray(buffer->begin_read() + _HeaderLen, meta_len)) {
            errorCode = EREQUEST;
        }

        // ================= parse user define message ====================
        if (errorCode == ENOERROR) {
            process_message(&meta, buffer->begin_read() + _HeaderLen + meta_len, body_len - meta_len);
            buffer->has_readed(_HeaderLen + body_len);
            continue;
        } else {
            // errorCallback_(conn, buf, receiveTime, errorCode);
            LOG_ERROR("parse rpc data error");
            TcpConnectionPtr conn(conn_weak_.lock());
            if (conn)
                conn->close();
        }
    }
}

void RpcChannel::process_message(const RpcMeta* meta, const char* msg, size_t n) {
    ErrorCode error = ENOERROR;
    int64_t id = meta->correlation_id();

    if (meta->has_response()) { // client recv response
        LOG_TRACE("proc response id: {}, code: {}, text: {}", id,
                  meta->response().error_code(), meta->response().error_text());

        RpcController* controller = nullptr;
        auto it = reqs_.find(id);
        if (it != reqs_.end()) {
            controller = it->second;
            reqs_.erase(it);
        }

        std::unique_ptr<RpcController> ctrl(controller);
        if (ctrl) {
            std::unique_ptr<google::protobuf::Message> resp(ctrl->get_resp());
            if (resp) {
                if (resp->ParseFromArray(msg, n)) {
                } else {
                    error = ERESPONSE;
                }

                // RpcController* ctrl = dynamic_cast<RpcMessage*>(out.ctrl);
                // ctrl.reset(out.ctrl);
            }

            auto it = _CodeError.find(error);
            if (it != _CodeError.end()) {
                ctrl->SetFailed(it->second);
            }

            // self delete
            if (nullptr != ctrl->get_done()) {
                ctrl->get_done()->Run();
            }
        }
    } else if (meta->has_request()) { // server recv request
        // XXX: extract to a function
        LOG_TRACE("proc request id: {}, service: {}, method: {}", id,
                  meta->request().service_name(), meta->request().method_name());
        do {
            if (nullptr == services_) {
                error = ENOSERVICE;
                break;
            }

            auto it = services_->find(meta->request().service_name());
            if (it == services_->end()) {
                error = ENOSERVICE;
                break;
            }

            google::protobuf::Service* service = it->second;
            // XXX: assert(service != NULL);
            const google::protobuf::ServiceDescriptor* desc = service->GetDescriptor();
            const google::protobuf::MethodDescriptor* method = desc->FindMethodByName(meta->request().method_name());
            if (nullptr == method) {
                error = ENOMETHOD;
                break;
            }

            std::unique_ptr<google::protobuf::Message> request(service->GetRequestPrototype(method).New());
            if (!request) {
                LOG_CRITICAL("memory alloc error");
                error = EINTERNAL; // XXX: abort();
                break;
            }

            if (!request->ParseFromArray(msg, n)) {
                error = EREQUEST;
                break;
            }

            google::protobuf::Message* response = service->GetResponsePrototype(method).New(); // delete in callback
            if (nullptr == response) {
                LOG_CRITICAL("memory alloc error");
                error = EINTERNAL; // // XXX: abort();
                break;
            }

            RpcController* ctrl = new (std::nothrow) RpcController; // delete in callback
            if (nullptr == ctrl) {
                LOG_CRITICAL("memory alloc error");
                delete response;
                error = EINTERNAL; // // XXX: abort();
                break;
            }

            auto done = google::protobuf::NewCallback(this, &RpcChannel::on_done, id, ctrl); // delete by self
            if (nullptr == done) {
                LOG_CRITICAL("memory alloc error");
                delete response;
                delete ctrl;
                error = EINTERNAL; // // XXX: abort();
                break;
            }

            ctrl->set_resp(response);
            ctrl->set_done(done);

            // call server's user-defined method
            service->CallMethod(method, ctrl, request.get(), response, done);
        } while (false);

        if (error != ENOERROR) {
            LOG_ERROR("proc request error: {}", error);
            send_resp(meta->correlation_id(), error, nullptr);
        }
    }
    //    else if (rpcmessage->type() == ERROR)
    //    {
    //        LOG_TRACE("message type error");
    //        TcpConnectionPtr conn(conn_weak_.lock());
    //        if (conn)
    //            conn->close();
    //    }
}

// this callback is called by user(sync or async in user's thread)
void RpcChannel::on_done(int64_t id, RpcController* ctrl) {
    ErrorCode error = ENOERROR;
    google::protobuf::Message* response = ctrl->get_resp();
    if (ctrl->Failed()) {
        LOG_TRACE("server method result error: {}", ctrl->ErrorText());
        error = EINTERNAL; // FIXME: process specific error
    }
    send_resp(id, error, response);

    delete response;
    delete ctrl;
    // done self delete
}

void RpcChannel::send_resp(int64_t id, int code, google::protobuf::Message* resp) {
    RpcMeta meta;
    meta.set_correlation_id(id);
    RpcResponseMeta* response_meta = meta.mutable_response();
    response_meta->set_error_code(code);
    auto it = _CodeError.find(code);
    if (it != _CodeError.end()) {
        response_meta->set_error_text(it->second);
    }

    // send response
    pack_send(&meta, resp);
}

void RpcChannel::pack_send(RpcMeta* meta, const google::protobuf::Message* msg) {
    BufferPtr send_buffer = std::make_shared<Buffer>();

    uint32_t meta_len = meta->ByteSizeLong();
    uint32_t msg_len = (msg != nullptr ? msg->ByteSizeLong() : 0); // not compress
    uint32_t body_len = meta_len + msg_len;                        // + attatchment

    send_buffer->ensure_writable(body_len + _HeaderLen);

    // ================= pack header ====================
    char header[_HeaderLen] = {"ERPC"};

    // 4-bytes body length
    // uint32_t be32 = boost::asio::detail::socket_ops::host_to_network_long(body_len);
    // send_buffer->write(reinterpret_cast<const char*>(&be32), 4);
    int32_t* temp = reinterpret_cast<int32_t*>(header + 4);
    *temp = boost::asio::detail::socket_ops::host_to_network_long(body_len);

    // 4-bytes meta length
    // be32 = boost::asio::detail::socket_ops::host_to_network_long(meta_len);
    // send_buffer->write(reinterpret_cast<const char*>(&be32), 4);
    temp = reinterpret_cast<int32_t*>(header + 8);
    *temp = boost::asio::detail::socket_ops::host_to_network_long(meta_len);

    send_buffer->write(header, _HeaderLen);

    // ================= pack body(meta) ====================
    uint8_t* start = reinterpret_cast<uint8_t*>(send_buffer->begin_write());
    uint8_t* end = meta->SerializeWithCachedSizesToArray(start);
    if (static_cast<uint32_t>(end - start) != meta_len) {
        LOG_ERROR("channel pack meta error {}:{}", end - start, meta_len);
        // ByteSizeConsistencyError(byte_size, message.ByteSize(), static_cast<int>(end - start));
    }
    send_buffer->has_written(meta_len);

    // ================= pack body(user defined message) ====================
    if (msg_len > 0) {
        start = reinterpret_cast<uint8_t*>(send_buffer->begin_write());
        end = msg->SerializeWithCachedSizesToArray(start);
        if (static_cast<uint32_t>(end - start) != msg_len) {
            LOG_ERROR("channel pack msg error {}:{}", end - start, msg_len);
            // ByteSizeConsistencyError(byte_size, message.ByteSize(), static_cast<int>(end - start));
        }
        send_buffer->has_written(msg_len);
    }

    // ================= pack body(attatchment) ====================

    TcpConnectionPtr conn(conn_weak_.lock());
    if (conn)
        conn->send(send_buffer);
}
