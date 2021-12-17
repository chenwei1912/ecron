#ifndef _RPC_CONTROLLER_H_
#define _RPC_CONTROLLER_H_


#include "google/protobuf/service.h"

#include <memory>



class RpcController : public google::protobuf::RpcController
{
public:
    RpcController();
    ~RpcController();

    // ----- user defined------
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
    std::string error_;
    int log_id_;
};

typedef std::shared_ptr<RpcController> RpcControllerPtr;

#endif // _RPC_CONTROLLER_H_