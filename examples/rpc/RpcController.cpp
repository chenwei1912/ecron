#include "RpcController.h"
#include "Logger.h"

using namespace ecron::net;

//static const std::unordered_map<int, std::string> _CodeError = {
//    { 0, "" },
//    { 1, "no service" },
//    { 2, "no method" },
//    { 3, "invalid request" },
//    { 4, "invalid response" },
//    { 5, "invoke timeout" },
//};


RpcController::RpcController()
    : response_(nullptr)
    , done_(nullptr)
    , error_code_(0)
    , log_id_(0) {
}

RpcController::~RpcController() {
    //LOG_TRACE("RpcController destructing");
}

void RpcController::reset()
{
    response_ = nullptr;
    done_ = nullptr;

    error_text_.clear();
    error_code_ = 0;
    log_id_ = 0;
}

void RpcController::set_error_code(int ec) {
    error_code_ = ec;
}

int RpcController::get_error_code() const {
    return error_code_;
}

void RpcController::set_log_id(int id)
{
    log_id_ = id;
}

int RpcController::get_log_id() const
{
    return log_id_;
}

void RpcController::Reset() {
    reset();
}

bool RpcController::Failed() const {
    return (0 != error_code_);
}

std::string RpcController::ErrorText() const {
    return error_text_;
};

void RpcController::StartCancel() {

}

void RpcController::SetFailed(const std::string& reason) {
    error_text_ = reason;
    error_code_ = -1; // FIXME: map code?
}

bool RpcController::IsCanceled() const {
    return false;
}

void RpcController::NotifyOnCancel(google::protobuf::Closure* callback) {

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

