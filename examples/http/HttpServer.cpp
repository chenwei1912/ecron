#include "HttpServer.h"
#include "Logger.h"

//#include <iostream>


using namespace ecron::net;


HttpServer::HttpServer(EventLoop* loop)
    : server_(loop, "HttpServer")
    , num_workers_(0)
{
    server_.set_connection_callback(std::bind(&HttpServer::on_connection, 
            this, std::placeholders::_1));
    server_.set_recv_callback(std::bind(&HttpServer::on_recv, this, 
            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    server_.set_sendcomplete_callback(std::bind(&HttpServer::on_sendcomplete, 
            this, std::placeholders::_1));
}

HttpServer::~HttpServer()
{
    workers_.stop();
}

bool HttpServer::start(const char* strip, unsigned short port, size_t n_io, size_t n_worker)
{
    bool ret = true;

    num_workers_ = n_worker;
    if (num_workers_ > 0) {
        if (0 != workers_.start(num_workers_, 100000)) {
            LOG_ERROR("HttpServer start workers failed!");
            return false;
        }
    }

    server_.set_io_threads(n_io);
    ret = server_.start(strip, port);
    if (!ret) {
        LOG_ERROR("HttpServer start listen failed!");
        workers_.stop();
        return false;
    }

    return ret;
}

void HttpServer::on_connection(const TcpConnectionPtr& conn)
{
    if (conn->connected()) {
        HttpTaskPtr task = std::make_shared<HttpTask>();
        task->init(conn);
        conn->set_context(task);

        task->set_http_callback(http_cb_);
        task->set_http_send_callback(http_send_cb_);

        // security: complete messge timeout
    }
    else {

    }
}

void HttpServer::on_recv(const TcpConnectionPtr& conn, Buffer* buffer, size_t len)
{
    LOG_TRACE("{}", buffer->begin_read());

    // no multiple requests at one time
    HttpTaskPtr task = boost::any_cast<HttpTaskPtr>(conn->get_context());
    if (!task->parse(buffer))
    {
        LOG_ERROR("parse http protocol error");
        conn->send("HTTP/1.1 400 Bad Request\r\n\r\n", 28);
        conn->close();
    }

    if (task->completed())
    {
        LOG_INFO("request parse OK! method: {}, url: {}", 
                            task->get_req()->get_method(), task->get_req()->get_url());
        if (num_workers_ > 0)
            workers_.append(std::bind(&HttpTask::on_request, task));
        else
            task->on_request();
    }
    //else
    //    LOG_TRACE("request parsed {}, need more data", task->get_req()->count_parsed_);
}

void HttpServer::on_sendcomplete(const TcpConnectionPtr& conn)
{
    HttpTaskPtr task = boost::any_cast<HttpTaskPtr>(conn->get_context());
    if (task->get_send_progressively()) {
        if (num_workers_ > 0)
            workers_.append(std::bind(&HttpTask::on_send_progressively, task));
        else
            task->on_send_progressively();
    }
}

