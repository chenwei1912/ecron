#ifndef ECRON_NET_HTTPTASK_H
#define ECRON_NET_HTTPTASK_H

#include "http_parser.h"
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

friend class HttpServer;

public:
    HttpTask();
    ~HttpTask();

    inline HttpRequest* get_req() { return &request_; }
    inline HttpResponse* get_resp() { return &response_; }

    inline void set_isbody(bool on) { is_body_ = on; }
    inline bool get_isbody() const { return is_body_; }
    
    void set_context(void* context) { context_ = context; }
    void* get_context() const { return context_; }

    // http parse callback
    static int on_message_begin(http_parser* parser);
    static int on_url(http_parser* parser, const char* at, size_t length);
    static int on_status(http_parser* parser, const char* at, size_t length);
    static int on_header_field(http_parser* parser, const char* at, size_t length);
    static int on_header_value(http_parser* parser, const char* at, size_t length);
    static int on_headers_complete(http_parser* parser);
    static int on_body(http_parser* parser, const char* at, size_t length);
    static int on_message_complete(http_parser* parser);

private:
    void init(const TcpConnectionPtr& conn);

    bool parse(Buffer* buffer);
    //bool parse(const char* pdata, size_t len);
    bool parse_postbody();

    inline bool completed() const { return completed_; }
    
    void on_request();
    void on_file();

    void set_http_callback(const HttpCallback& cb)
    {
        http_cb_ = cb;
    }
    void set_http_body_callback(const HttpBodyCallback& cb)
    {
        http_body_cb_ = cb;
    }

    std::weak_ptr<TcpConnection> conn_weak_;

    //http_parser_settings settings_;
    http_parser parser_;
    std::string header_field_; //field is waiting for value while parsing
    //bool headercomplete_;
    bool completed_;
    //size_t count_parsed_;

    HttpRequest request_;
    HttpResponse response_;

    HttpCallback http_cb_;
    HttpBodyCallback http_body_cb_;
    bool is_body_;
    void* context_;
};

typedef std::shared_ptr<HttpTask> HttpTaskPtr;

}// namespace net
}// namespace ecron

#endif // ECRON_NET_HTTPTASK_H

