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
//    : loop_(loop)
//    , server_(loop, "HttpServer")
{
}

RpcController::~RpcController()
{
    LOG_TRACE("RpcController destructing");
}

void RpcController::reset()
{
    response_ = nullptr;
    done_ = nullptr;

    error_.clear();
    log_id_ = 0;
}

void RpcController::set_id(int id)
{
    log_id_ = id;
}

int RpcController::get_id() const
{
    return log_id_;
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

