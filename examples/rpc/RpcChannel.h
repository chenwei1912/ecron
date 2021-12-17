#ifndef _RPC_CHANNEL_H_
#define _RPC_CHANNEL_H_


#include "TcpConnection.h"

#include "google/protobuf/service.h"

#include <memory>
#include <unordered_map>
#include <utility>


class RpcMessage;
typedef std::shared_ptr<RpcMessage> RpcMessagePtr;

//typedef std::pair<::google::protobuf::Message*, ::google::protobuf::Closure*> OutstandingCall;

class RpcChannel : public ::google::protobuf::RpcChannel
{
public:
    RpcChannel();
    ~RpcChannel();

    void set_conn(const netlib::TcpConnectionPtr& conn);
    void set_services(std::unordered_map<std::string, ::google::protobuf::Service*>* services);

    void CallMethod(const ::google::protobuf::MethodDescriptor* method,
                  ::google::protobuf::RpcController* controller,
                  const ::google::protobuf::Message* request,
                  ::google::protobuf::Message* response,
                  ::google::protobuf::Closure* done);

    void process(netlib::Buffer* buffer, size_t len);
    void process_message(const RpcMessagePtr& messagePtr);

private:
    struct OutstandingData
    {
        google::protobuf::RpcController* ctrl;
        google::protobuf::Message* response;
        google::protobuf::Closure* done;
    };

    void on_done(OutstandingData out, int64_t id);
    void send_resp(int64_t id, int code, google::protobuf::Message* resp);
    void pack_send(RpcMessage* msg);

    std::unordered_map<int64_t, OutstandingData> outstandings_;

    std::atomic<int64_t> id_;
    netlib::TcpConnectionPtr conn_;
    //std::weak_ptr<netlib::TcpConnection> conn_weak_;
    std::unordered_map<std::string, ::google::protobuf::Service*>* services_;
};

typedef std::shared_ptr<RpcChannel> RpcChannelPtr;

#endif // _RPC_CHANNEL_H_