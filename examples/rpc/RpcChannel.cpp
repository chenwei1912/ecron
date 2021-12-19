#include "RpcChannel.h"
#include "Logger.h"

#include "RpcController.h"

#include "google/protobuf/descriptor.h"
#include "boost/asio.hpp"

#include "rpc.pb.h"


static const std::unordered_map<int, std::string> _CodeError = {
    { 0, "" },
    { 1, "no service" },
    { 2, "no method" },
    { 3, "invalid request" },
    { 4, "invalid response" },
    { 5, "invoke timeout" },
};

static const size_t _HeaderLen = 2;



typedef std::shared_ptr<google::protobuf::Message> MessagePtr;



RpcChannel::RpcChannel()
//    : loop_(loop)
//    , server_(loop, "HttpServer")
{
}

RpcChannel::~RpcChannel()
{
    LOG_TRACE("RpcChannel destructing");
    for (const auto& item : outstandings_)
    {
        OutstandingData out = item.second;
        delete out.ctrl;
        delete out.response;
        delete out.done;
    }
}

void RpcChannel::set_conn(const netlib::TcpConnectionPtr& conn)
{
    conn_weak_ = conn;
}

void RpcChannel::on_recv(const netlib::TcpConnectionPtr& conn, netlib::Buffer* buffer, size_t len)
{
    process(buffer, len);
}

void RpcChannel::set_services(std::unordered_map<std::string, ::google::protobuf::Service*>* services)
{
    services_ = services;
}

// client send request(prepare response object)
void RpcChannel::CallMethod(const ::google::protobuf::MethodDescriptor* method,
              google::protobuf::RpcController* controller,
              const google::protobuf::Message* request,
              google::protobuf::Message* response,
              google::protobuf::Closure* done)
{
    // packed request
    RpcMessage message;
    message.set_type(REQUEST);
    int64_t id = id_.fetch_add(1);
    message.set_id(id);
    message.set_service(method->service()->full_name());
    message.set_method(method->name());
    message.set_request(request->SerializeAsString());

    // save request associated info
    OutstandingData out = { controller, response, done };
    outstandings_[id] = out;

    // send request
    pack_send(&message);
}

void RpcChannel::process(netlib::Buffer* buffer, size_t len)
{
    // process rpc meta message
    while (buffer->readable_bytes() >= _HeaderLen)
    {
        uint16_t msg_len = 0;
        memcpy(&msg_len, buffer->begin_read(), _HeaderLen);
        msg_len = boost::asio::detail::socket_ops::network_to_host_short(msg_len);

        // header_len range?
        if (buffer->readable_bytes() < msg_len)
            return;

        const google::protobuf::Descriptor* descriptor = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName("RpcMessage");
        const google::protobuf::Message* prototype = google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);

        RpcMessagePtr msg(dynamic_cast<RpcMessage*>(prototype->New())); // only RpcMessage
        ErrorCode errorCode = NO_ERROR;
        if (!msg->ParseFromArray(buffer->begin_read() + _HeaderLen, msg_len - _HeaderLen))
            errorCode = INVALID_REQUEST;
        //ErrorCode errorCode = parse(buf->peek()+kHeaderLen, len, message.get());
        if (errorCode == NO_ERROR)
        {
            // process rpc message
            process_message(msg);
            buffer->has_readed(msg_len);
            continue;
        }
        else
        {
            //errorCallback_(conn, buf, receiveTime, errorCode);
            LOG_ERROR("RpcChannel decode data error");
            netlib::TcpConnectionPtr conn(conn_weak_.lock());
            if (conn)
                conn->close();
        }
    }
}

void RpcChannel::process_message(const RpcMessagePtr& rpcmessage)
{
    //RpcMessage& message = *messagePtr;
    if (rpcmessage->type() == RESPONSE) // client recv response
    {
        int64_t id = rpcmessage->id();
        //assert(message.has_response() || message.has_error());

        //::google::protobuf::Message* response = nullptr;
        //::google::protobuf::Closure* done = nullptr;
        OutstandingData out = { nullptr, nullptr, nullptr };
        {
            auto it = outstandings_.find(id);
            if (it != outstandings_.end())
            {
                out = it->second;
                outstandings_.erase(it);
            }
        }

        if (nullptr != out.response)
        {
            std::unique_ptr<google::protobuf::Message> resp(out.response);
            //if (message.has_response())
            out.response->ParseFromString(rpcmessage->response());

            std::unique_ptr<google::protobuf::RpcController> ctrl;
            if (nullptr != out.ctrl)
            {
                //RpcController* ctrl = dynamic_cast<RpcMessage*>(out.ctrl);
                ctrl.reset(out.ctrl);

                auto it = _CodeError.find(static_cast<int>(rpcmessage->error()));
                if (it != _CodeError.end())
                    ctrl->SetFailed(it->second);
            }

            // invoke response callback
            if (nullptr != out.done)
                out.done->Run();

        }
    }
    else if (rpcmessage->type() == REQUEST) // server recv request
    {
        // FIXME: extract to a function
        ErrorCode error = NO_ERROR;
        if (nullptr != services_)
        {
            auto it = services_->find(rpcmessage->service());
            if (it != services_->end())
            {
                google::protobuf::Service* service = it->second;
                //assert(service != NULL);
                const google::protobuf::ServiceDescriptor* desc = service->GetDescriptor();
                const google::protobuf::MethodDescriptor* method = desc->FindMethodByName(rpcmessage->method());
                if (nullptr != method)
                {
                    std::unique_ptr<google::protobuf::Message> request(service->GetRequestPrototype(method).New());
                    if (request->ParseFromString(rpcmessage->request()))
                    {
                        std::unique_ptr<google::protobuf::Message> response(service->GetResponsePrototype(method).New());
                        std::unique_ptr<google::protobuf::RpcController> ctrl(new RpcController);
                        int64_t id = rpcmessage->id();
                        OutstandingData out = { ctrl.get(), response.get(), nullptr };

                        // in done->Run(), self delete
                        auto done = google::protobuf::NewCallback(this, &RpcChannel::on_done, out, id);

                        // call the server's method
                        // done->Run() is invoke in service->CallMethod
                        service->CallMethod(method, ctrl.get(), request.get(), response.get(), done);
                        error = NO_ERROR;
                    }
                    else
                    {
                        error = INVALID_REQUEST;
                    }
                }
                else
                {
                    error = NO_METHOD;
                }
            }
            else
            {
                error = NO_SERVICE;
            }
        }
        else
        {
           error = NO_SERVICE;
        }

        if (error != NO_ERROR)
        {
            send_resp(rpcmessage->id(), error, nullptr);
        }
    }
    else if (rpcmessage->type() == ERROR)
    {
        LOG_TRACE("message type error");
        netlib::TcpConnectionPtr conn(conn_weak_.lock());
        if (conn)
            conn->close();
    }
}

// this callback is for server side after user define method invoke
void RpcChannel::on_done(OutstandingData out, int64_t id)
{
    // process result use out.ctrl(RpcController)
//    ErrorCode error = NO_ERROR;
//    if (nullptr != out.ctrl)
//    {
//        if (out.ctrl->Failed())
//            error = INVALID_REQUEST;
//    }

    send_resp(id, NO_ERROR, out.response);
}

void RpcChannel::send_resp(int64_t id, int code, google::protobuf::Message* resp)
{
    RpcMessage message;
    message.set_type(RESPONSE);
    message.set_id(id);
    message.set_error(static_cast<ErrorCode>(code));
    if (nullptr != resp)
        message.set_response(resp->SerializeAsString()); // FIXME: error check
    
    // send response
    pack_send(&message);
}

void RpcChannel::pack_send(RpcMessage* msg)
{
    netlib::BufferPtr send_buffer = std::make_shared<netlib::Buffer>();

    uint16_t msg_len = msg->ByteSizeLong() + _HeaderLen;
    send_buffer->ensure_writable(msg_len);

    uint16_t be16 = boost::asio::detail::socket_ops::host_to_network_short(msg_len);
    send_buffer->write(reinterpret_cast<const char*>(&be16), _HeaderLen);

    uint8_t* start = reinterpret_cast<uint8_t*>(send_buffer->begin_write());
    uint8_t* end = msg->SerializeWithCachedSizesToArray(start);
    if (end - start != msg_len - _HeaderLen)
    {
        LOG_TRACE("RpcChannel pack_send error {}:{}", end - start, msg_len - _HeaderLen);
        //ByteSizeConsistencyError(byte_size, message.ByteSize(), static_cast<int>(end - start));
    }
    send_buffer->has_written(msg_len - _HeaderLen);

    netlib::TcpConnectionPtr conn(conn_weak_.lock());
    if (conn)
        conn->send(send_buffer);
}
