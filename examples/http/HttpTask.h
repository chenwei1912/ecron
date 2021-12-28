#ifndef ECRON_NET_HTTPTASK_H
#define ECRON_NET_HTTPTASK_H

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "TcpConnection.h"
#include "Buffer.h"


namespace ecron
{
namespace net
{

class HttpTask;
typedef std::function<void(HttpTask*)> HttpCallback;
typedef std::function<void(HttpTask*, Buffer*)> HttpBodyCallback;
    
class HttpTask // represent a request-response process
{
public:
    HttpTask();
    ~HttpTask();

    void init(const TcpConnectionPtr& conn);

    bool parse(Buffer* buffer);
    //int parse(const char* pdata, uint32_t len);
    //void send_complete(const ecron::net::TcpConnectionPtr& conn);

    //void process();
    //void process_body();
    void on_request();
    void on_body();

    inline bool is_complete() const { return request_.msgcomplete_; }

    inline void set_isbody(bool on) { is_body_ = on; }
    inline bool get_isbody() const { return is_body_; }

    inline const HttpRequest* get_req() { return &request_; }
    inline HttpResponse* get_resp() { return &response_; }

    void set_http_callback(const HttpCallback& cb)
    {
        http_cb_ = cb;
    }
    void set_http_body_callback(const HttpBodyCallback& cb)
    {
        http_body_cb_ = cb;
    }

    void set_context(void* context) { context_ = context; }
    void* get_context() const { return context_; }
    
private:
    //void process_header(ecron::Buffer* buffer);
    //void do_post();
    //bool user_verify(const std::string& name, const std::string& pwd, int login);

    std::weak_ptr<TcpConnection> conn_weak_;
    
    HttpRequest request_;
    HttpResponse response_;

    HttpCallback http_cb_;
    HttpBodyCallback http_body_cb_;

    bool is_body_;
    //FILE* file_;

    void* context_;
};

typedef std::shared_ptr<HttpTask> HttpTaskPtr;

}// namespace net
}// namespace ecron

#endif // ECRON_NET_HTTPTASK_H

