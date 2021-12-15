#include "RpcChannelImpl.h"
#include "Logger.h"

#include "google/protobuf/descriptor.h"

#include "rpc.pb.h"


static const std::unordered_map<int, std::string> _CodeError = {
    { 0, "" },
    { 1, "no service" },
    { 2, "no method" },
    { 3, "invalid request" },
    { 4, "invalid response" },
    { 5, "invoke timeout" },
};

typedef std::shared_ptr<google::protobuf::Message> MessagePtr;


RpcController::RpcController()
//    : loop_(loop)
//    , server_(loop, "HttpServer")
{
}

RpcController::~RpcController()
{
    LOG_TRACE("RpcController destructing");
}

//std::string RpcController::ErrorText() const
//{
//    std::string str;
//    switch (error_code_)
//    {
//    case 0:
//        str = "";
//        break;
//    case 1:
//        str = "";
//        break;
//    case 2:
//        str = "";
//        break;
//    case 3:
//        str = "";
//        break;
//    case 4:
//        str = "";
//        break;
//    case 5:
//        str = "";
//        break;
//    default:
//        break;
//    }

//    return str;
//}

RpcChannelImpl::RpcChannelImpl()
//    : loop_(loop)
//    , server_(loop, "HttpServer")
{
}

RpcChannelImpl::~RpcChannelImpl()
{
    LOG_TRACE("RpcChannelImpl destructing");
    for (const auto& item : outstandings_)
    {
        OutstandingCall out = item.second;
        delete out.ctrl;
        delete out.response;
        delete out.done;
    }
}

void RpcChannelImpl::set_conn(const netlib::TcpConnectionPtr& conn)
{
    conn_ = conn;
}

void RpcChannelImpl::set_services(std::unordered_map<std::string, ::google::protobuf::Service*>* services)
{
    services_ = services;
}

// client send request(prepare response object)
void RpcChannelImpl::CallMethod(const ::google::protobuf::MethodDescriptor* method,
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
    OutstandingCall out = { controller, response, done };
    outstandings_[id] = out;

    // send request
    pack_send(&message);
}

void RpcChannelImpl::process(netlib::Buffer* buffer, size_t len)
{
    // process rpc meta message
    const google::protobuf::Descriptor* descriptor = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName("RpcMessage");
    const google::protobuf::Message* prototype = google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);

    RpcMessagePtr msg(dynamic_cast<RpcMessage*>(prototype->New())); // only RpcMessage
    ErrorCode errorCode = NO_ERROR;
    if (!msg->ParseFromArray(buffer->begin_read(), buffer->readable_bytes()))
        errorCode = INVALID_REQUEST;
    //ErrorCode errorCode = parse(buf->peek()+kHeaderLen, len, message.get());
    if (errorCode == NO_ERROR)
    {
        // process rpc message
        process_message(msg);
        buffer->has_readall();
    }
    else
    {
        //errorCallback_(conn, buf, receiveTime, errorCode);
        conn_->close();
    }
}

void RpcChannelImpl::process_message(const RpcMessagePtr& messagePtr)
{
    RpcMessage& message = *messagePtr;
    if (message.type() == RESPONSE) // client recv response
    {
        int64_t id = message.id();
        //assert(message.has_response() || message.has_error());

        //::google::protobuf::Message* response = nullptr;
        //::google::protobuf::Closure* done = nullptr;
        OutstandingCall out = { nullptr, nullptr, nullptr };
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
            std::unique_ptr<google::protobuf::Message> d(out.response);
            if (message.has_response())
                out.response->ParseFromString(message.response());

            std::unique_ptr<google::protobuf::RpcController> ctrl;
            if (nullptr != out.ctrl && message.has_error())
            {
                //RpcController* ctrl = dynamic_cast<RpcMessage*>(out.ctrl);
                ctrl.reset(out.ctrl);

                auto it = _CodeError.find(static_cast<int>(message.error()));
                if (it != _CodeError.end())
                    ctrl->SetFailed(it->second);
            }

            // invoke response callback
            if (nullptr != out.done)
                out.done->Run();

            //no need call Reset because resoure is released
            //ctrl->Reset();
        }
    }
    else if (message.type() == REQUEST) // server recv request
    {
        // FIXME: extract to a function
        ErrorCode error = NO_ERROR;
        if (nullptr != services_)
        {
            auto it = services_->find(message.service());
            if (it != services_->end())
            {
                google::protobuf::Service* service = it->second;
                //assert(service != NULL);
                const google::protobuf::ServiceDescriptor* desc = service->GetDescriptor();
                const google::protobuf::MethodDescriptor* method = desc->FindMethodByName(message.method());
                if (nullptr != method)
                {
                    std::unique_ptr<google::protobuf::Message> request(service->GetRequestPrototype(method).New());
                    if (request->ParseFromString(message.request()))
                    {
                        // response is deleted in doneCallback
                        google::protobuf::Message* response = service->GetResponsePrototype(method).New();
                        int64_t id = message.id();

                        RpcController* ctrl = new RpcController;
                        OutstandingCall out = { ctrl, response, nullptr };
                        auto done = google::protobuf::NewCallback(this, &RpcChannelImpl::on_done, out, id);

                        // call the user define rpc method
                        service->CallMethod(method, NULL, request.get(), response, done);
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
            RpcMessage response;
            response.set_type(RESPONSE);
            response.set_id(message.id());
            response.set_error(error);

            pack_send(&message);
        }
    }
    else if (message.type() == ERROR)
    {
        //log
        conn_->close();
    }
}

// this callback is for server side after user define method invoke
void RpcChannelImpl::on_done(OutstandingCall out, int64_t id)
{
    std::unique_ptr<google::protobuf::Message> d(out.response);
    RpcMessage message;
    message.set_type(RESPONSE);
    message.set_id(id);

    // out.ctrl has method exec result
    std::unique_ptr<google::protobuf::RpcController> c(out.ctrl);
//    if (out.ctrl->Failed())
//        message.set_error(NO_ERROR);
//    else
    message.set_error(NO_ERROR);
    message.set_response(out.response->SerializeAsString()); // FIXME: error check
    
    // send response
    pack_send(&message);
}

void RpcChannelImpl::pack_send(RpcMessage* msg)
{
    netlib::BufferPtr send_buffer = std::make_shared<netlib::Buffer>();

    int byte_size = msg->ByteSize();
    send_buffer->ensure_writable(byte_size);

    uint8_t* start = reinterpret_cast<uint8_t*>(send_buffer->begin_write());
    uint8_t* end = msg->SerializeWithCachedSizesToArray(start);
    if (end - start != byte_size)
    {
        // log error
        //ByteSizeConsistencyError(byte_size, message.ByteSize(), static_cast<int>(end - start));
    }
    send_buffer->has_written(byte_size);

    conn_->send(send_buffer);
}
