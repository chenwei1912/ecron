#ifndef ECRON_NET_RPCCONTROLLER_H
#define ECRON_NET_RPCCONTROLLER_H


#include "google/protobuf/service.h"

#include <memory>


namespace ecron
{
namespace net
{

class RpcController : public google::protobuf::RpcController
{
public:
    RpcController();
    ~RpcController();

    // ----- user defined------
    void reset();

    void set_resp(google::protobuf::Message* resp) { response_ = resp; }
    google::protobuf::Message* get_resp() const { return response_; }

    void set_done(google::protobuf::Closure* done) { done_ = done; }
    google::protobuf::Closure* get_done() const { return done_; }
    
    void set_id(int id);
    int get_id() const;

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

    google::protobuf::Message* response_;
    google::protobuf::Closure* done_;
        
    std::string error_;
    int log_id_;
};

typedef std::shared_ptr<RpcController> RpcControllerPtr;

}// namespace net
}// namespace ecron

#endif // ECRON_NET_RPCCONTROLLER_H