#ifndef _RPC_CHANNEL_H_
#define _RPC_CHANNEL_H_


#include "TcpConnection.h"

#include "google/protobuf/service.h"

#include <memory>
#include <unordered_map>
#include <utility>


class RpcMessage;
typedef std::shared_ptr<RpcMessage> RpcMessagePtr;

class RpcController;

//typedef std::pair<::google::protobuf::Message*, ::google::protobuf::Closure*> OutstandingCall;

class RpcChannel : public google::protobuf::RpcChannel
{
public:
    RpcChannel();
    ~RpcChannel();

	RpcChannel(const RpcChannel&) = delete;
	RpcChannel& operator=(const RpcChannel&) = delete;

//	RpcChannel(RpcChannel&&) = default;
//  RpcChannel& operator=(RpcChannel&&) = default;

    // ----- both side------
    void set_conn(const netlib::TcpConnectionPtr& conn);
    void on_recv(const netlib::TcpConnectionPtr& conn, netlib::Buffer* buffer, size_t len);

    // ----- server side------
    void set_services(std::unordered_map<std::string, ::google::protobuf::Service*>* services);

    // ----- client side------
    void CallMethod(const google::protobuf::MethodDescriptor* method,
                      google::protobuf::RpcController* controller,
                      const ::google::protobuf::Message* request,
                      google::protobuf::Message* response,
                      google::protobuf::Closure* done);
private:
    // ----- both side------
    struct OutstandingData
    {
        google::protobuf::RpcController* ctrl;
        google::protobuf::Message* response;
        google::protobuf::Closure* done;
    };

    void process(netlib::Buffer* buffer, size_t len);
    void process_message(const RpcMessagePtr& rpcmessage);
    void pack_send(RpcMessage* msg);

    //netlib::TcpConnectionPtr conn_;
    std::weak_ptr<netlib::TcpConnection> conn_weak_;
    std::unordered_map<int64_t, RpcController*> reqs_;

    // ----- server side------
    void on_done(int64_t id, RpcController* ctrl);
    void send_resp(int64_t id, int code, google::protobuf::Message* resp);
        
    std::unordered_map<std::string, ::google::protobuf::Service*>* services_;

    // ----- client side------
    std::atomic<int64_t> id_;

};

typedef std::shared_ptr<RpcChannel> RpcChannelPtr;

#endif // _RPC_CHANNEL_H_