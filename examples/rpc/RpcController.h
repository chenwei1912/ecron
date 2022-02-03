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

    inline void set_resp(google::protobuf::Message* resp) {
        response_ = resp;
    }
    inline google::protobuf::Message* get_resp() const {
        return response_;
    }

    inline void set_done(google::protobuf::Closure* done) {
        done_ = done;
    }
    inline google::protobuf::Closure* get_done() const {
        return done_;
    }

    void set_error_code(int ec);
    int get_error_code() const;

    void set_log_id(int id);
    int get_log_id() const;

    // ----- client side------
    virtual void Reset();
    virtual bool Failed() const;
    virtual std::string ErrorText() const;
    virtual void StartCancel();

    // ----- server side------
    virtual void SetFailed(const std::string& reason);
    virtual bool IsCanceled() const;
    virtual void NotifyOnCancel(google::protobuf::Closure* callback);

private:

    google::protobuf::Message* response_;
    google::protobuf::Closure* done_;
        
    std::string error_text_;
    int error_code_;
    int log_id_;
};

typedef std::shared_ptr<RpcController> RpcControllerPtr;

}// namespace net
}// namespace ecron

#endif // ECRON_NET_RPCCONTROLLER_H