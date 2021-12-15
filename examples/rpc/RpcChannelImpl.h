#ifndef _RPC_CHANNELIMPL_H_
#define _RPC_CHANNELIMPL_H_


#include "TcpConnection.h"

#include "google/protobuf/service.h"

#include <memory>
#include <unordered_map>
#include <utility>



class RpcController : public google::protobuf::RpcController
{
public:
    RpcController();
    ~RpcController();

    // ----- client side------
    virtual void Reset() { error_.clear(); }
    virtual bool Failed() const { return !error_.empty(); }
    virtual std::string ErrorText() const { return error_; };
    virtual void StartCancel() {}

    // ----- server side------
    virtual void SetFailed(const std::string& reason) { error_ = reason; }
    virtual bool IsCanceled() const { return false; }
    virtual void NotifyOnCancel(google::protobuf::Closure* callback) {}

private:
    std::string error_;
};


class RpcMessage;
typedef std::shared_ptr<RpcMessage> RpcMessagePtr;

//typedef std::pair<::google::protobuf::Message*, ::google::protobuf::Closure*> OutstandingCall;

class RpcChannelImpl : public ::google::protobuf::RpcChannel
{
public:
    RpcChannelImpl();
    ~RpcChannelImpl();

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
    struct OutstandingCall
    {
        google::protobuf::RpcController* ctrl;
        google::protobuf::Message* response;
        google::protobuf::Closure* done;
    };

    void on_done(OutstandingCall out, int64_t id);
    void pack_send(RpcMessage* msg);

    std::unordered_map<int64_t, OutstandingCall> outstandings_;

    std::atomic<int64_t> id_;
    netlib::TcpConnectionPtr conn_;
    //std::weak_ptr<netlib::TcpConnection> conn_weak_;
    std::unordered_map<std::string, ::google::protobuf::Service*>* services_;
};

typedef std::shared_ptr<RpcChannelImpl> RpcChannelImplPtr;

#endif // _RPC_CHANNELIMPL_H_
