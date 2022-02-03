#ifndef ECRON_NET_RPCCHANNEL_H
#define ECRON_NET_RPCCHANNEL_H


#include "TcpConnection.h"

#include "google/protobuf/service.h"

#include <memory>
#include <unordered_map>
#include <utility>



namespace ecron
{

class RpcMeta;

namespace net
{

class RpcController;

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
    void set_conn(const TcpConnectionPtr& conn);
    void process(Buffer* buffer, size_t len);

    // ----- server side------
    void set_services(std::unordered_map<std::string, google::protobuf::Service*>* services);

    // ----- client side------
    void CallMethod(const google::protobuf::MethodDescriptor* method,
                      google::protobuf::RpcController* controller,
                      const google::protobuf::Message* request,
                      google::protobuf::Message* response,
                      google::protobuf::Closure* done);
private:
    // ----- both side------
//    struct OutstandingData
//    {
//        google::protobuf::RpcController* ctrl;
//        google::protobuf::Message* response;
//        google::protobuf::Closure* done;
//    };

    void process_message(const RpcMeta* meta, const char* msg, size_t n);
    //void process_request(const RpcMeta* meta, const char* msg, size_t n);
    //void process_response(const RpcMeta* meta, const char* msg, size_t n);
    
    void pack_send(RpcMeta* meta, const google::protobuf::Message* msg);

    //netlib::TcpConnectionPtr conn_;
    std::weak_ptr<TcpConnection> conn_weak_;
    std::unordered_map<int64_t, RpcController*> reqs_;

    // ----- server side------
    void on_done(int64_t id, RpcController* ctrl);
    void send_resp(int64_t id, int code, google::protobuf::Message* resp);
        
    std::unordered_map<std::string, google::protobuf::Service*>* services_;

    // ----- client side------
    std::atomic<int64_t> id_;

};

typedef std::shared_ptr<RpcChannel> RpcChannelPtr;

}// namespace net
}// namespace ecron

#endif // ECRON_NET_RPCCHANNEL_H