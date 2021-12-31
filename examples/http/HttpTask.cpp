#include "HttpTask.h"
#include "Logger.h"

//#include <cstring>
//#include <climits>

#include <unordered_set>
#include <unordered_map>


using namespace ecron;
using namespace ecron::net;

//static const std::unordered_map<std::string, std::string> _SuffixType = {
//    { ".html",  "text/html" },
//    { ".xml",   "text/xml" },
//    { ".xhtml", "application/xhtml+xml" },
//    { ".txt",   "text/plain" },
//    { ".rtf",   "application/rtf" },
//    { ".pdf",   "application/pdf" },
//    { ".word",  "application/nsword" },
//    { ".png",   "image/png" },
//    { ".gif",   "image/gif" },
//    { ".jpg",   "image/jpeg" },
//    { ".jpeg",  "image/jpeg" },
//    { ".ico",   "image/x-icon"},
//    { ".au",    "audio/basic" },
//    { ".mpeg",  "video/mpeg" },
//    { ".mpg",   "video/mpeg" },
//    { ".avi",   "video/x-msvideo" },
//    { ".gz",    "application/x-gzip" },
//    { ".tar",   "application/x-tar" },
//    { ".css",   "text/css "},
//    { ".js",    "text/javascript "}
//};

//static const std::unordered_map<int, std::string> _CodeStatus = {
//    { 200, "OK" },
//    { 400, "Bad Request" },
//    { 403, "Forbidden" },
//    { 404, "Not Found" },
//};

//static const std::unordered_map<int, std::string> _ErrorCodePath = {
//    { 400, "/400.html" },
//    { 403, "/403.html" },
//    { 404, "/404.html" },
//};

//static const std::unordered_set<std::string> _Route = {
//"/index",
//"/login",
//"/register",
//"/picture",
//"/video",
//"/fans"
//};

const http_parser_settings _parser_settings = {
    &HttpTask::on_message_begin,
    &HttpTask::on_url,
    &HttpTask::on_status,
    &HttpTask::on_header_field,
    &HttpTask::on_header_value,
    &HttpTask::on_headers_complete,
    &HttpTask::on_body,
    &HttpTask::on_message_complete
};

//void dump_url(const char *url, const struct http_parser_url *u) {
//    unsigned int i;
//    printf("\tfield_set: 0x%x, port: %u\n", u->field_set, u->port);
//    for (i = 0; i < UF_MAX; i++) {
//        if ((u->field_set & (1 << i)) == 0) {
//            printf("\tfield_data[%u]: unset\n", i);
//            continue;
//        }
//        printf("\tfield_data[%u]: off: %u, len: %u, part: %.*s\n", i, u->field_data[i].off
//                , u->field_data[i].len, u->field_data[i].len, url + u->field_data[i].off);
//    }
//}

int HttpTask::on_message_begin(http_parser* parser)
{
    //printf("\n***MESSAGE BEGIN***\n\n");
    return 0;
}

int HttpTask::on_url(http_parser* parser, const char* at, size_t length)
{
    HttpTask* task = (HttpTask*)parser->data;
    task->request_.url_.assign(at, length);

//    http_parser_url u;
//    http_parser_url_init(&u);
//    u.port = 80;

//    int reval = http_parser_parse_url(at, length, 0, &u);
//    if (0 == reval) {
//        if (u.field_set & (1 << UF_SCHEMA))
//            wrapper->url_.schema.assign(at + u.field_data[UF_SCHEMA].off, u.field_data[UF_SCHEMA].len);

//        if (u.field_set & (1 << UF_HOST))
//            wrapper->url_.host.assign(at + u.field_data[UF_HOST].off, u.field_data[UF_HOST].len);

//        if (u.field_set & (1 << UF_PORT))
//            wrapper->url_.port = u.port;

//        if (u.field_set & (1 << UF_PATH))
//            wrapper->url_.path.assign(at + u.field_data[UF_PATH].off, u.field_data[UF_PATH].len);

//        if (u.field_set & (1 << UF_QUERY))
//            wrapper->url_.query.assign(at + u.field_data[UF_QUERY].off, u.field_data[UF_QUERY].len);

//        if (u.field_set & (1 << UF_FRAGMENT))
//            wrapper->url_.fragment.assign(at + u.field_data[UF_FRAGMENT].off, u.field_data[UF_FRAGMENT].len);

//        if (u.field_set & (1 << UF_USERINFO))
//            wrapper->url_.userinfo.assign(at + u.field_data[UF_USERINFO].off, u.field_data[UF_USERINFO].len);
//    }

    //printf("Url: %.*s\n", (int)length, at);

    return 0;
}

int HttpTask::on_status(http_parser* parser, const char* at, size_t length)
{
    //printf("Status: %.*s\n", (int)length, at);
    return 0;
}

int HttpTask::on_header_field(http_parser* parser, const char* at, size_t length)
{
    HttpTask* task = (HttpTask*)parser->data;
    task->header_field_.assign(at, length);

    //printf("Header field: %.*s\n", (int)length, at);
    return 0;
}

int HttpTask::on_header_value(http_parser* parser, const char* at, size_t length)
{
    HttpTask* task = (HttpTask*)parser->data;
    task->request_.headers_[task->header_field_] = std::string(at, length);

    //printf("Header value: %.*s\n", (int)length, at);
    return 0;
}

int HttpTask::on_headers_complete(http_parser* parser)
{
    HttpTask* task = (HttpTask*)parser->data;
    task->request_.method_ = http_method_str((http_method)parser->method);
    if (0 == http_should_keep_alive(parser))
        task->request_.keep_alive_ = false;
    else
        task->request_.keep_alive_ = true;

    //printf("\n***HEADERS COMPLETE***\n\n");
    return 0;
}

int HttpTask::on_body(http_parser* parser, const char* at, size_t length)
{
    // this callback function is probably called more than once

    HttpTask* task = (HttpTask*)parser->data;
    task->request_.body_.write(at, length);
    //printf("Body: %.*s\n", (int)length, at);
    return 0;
}

int HttpTask::on_message_complete(http_parser* parser)
{
    HttpTask* task = (HttpTask*)parser->data;
    task->completed_ = true;

    //printf("\n***MESSAGE COMPLETE***\n\n");
    return 0;
}

//static int HttpTask::on_chunk_header(http_parser* parser)
//{
//    return 0;
//}

//static int HttpTask::on_chunk_complete(http_parser* parser)
//{
//    return 0;
//}

static void defaultHttpCallback(HttpTask* task)
{

}
static void defaultHttpBodyCallback(HttpTask* task, Buffer* buffer)
{

}


HttpTask::HttpTask()
    : http_cb_(defaultHttpCallback)
    , http_body_cb_(defaultHttpBodyCallback)
{
}

HttpTask::~HttpTask()
{
    //LOG_TRACE("HttpConn destructing");
    //if (nullptr != file_)
    //    fclose(file_);
}

void HttpTask::init(const TcpConnectionPtr& conn)
{
    conn_weak_ = conn;

    http_parser_init(&parser_, HTTP_REQUEST/*HTTP_BOTH*/);
    parser_.data = this;
    header_field_.clear();
    completed_ = false;
    //count_parsed_ = 0;

    request_.init();
    response_.init();
    
    is_body_ = false;
    context_ = nullptr;
}

bool HttpTask::parse(Buffer* buffer)
{
    size_t nparsed = 0;
    nparsed = http_parser_execute(&parser_, &_parser_settings, 
                                    buffer->begin_read(), buffer->readable_bytes());
    if (HPE_OK != HTTP_PARSER_ERRNO(&parser_)) {
        return false;
    }

    //count_parsed_ += nparsed;
    buffer->has_readed(nparsed);
    return true;
}

//bool HttpRequest::parse(const char* pdata, uint32_t len)
//{
//    size_t nparsed = 0;
//    nparsed = http_parser_execute(&parser_, &_parser_settings, pdata, len);
//    if (HPE_OK != HTTP_PARSER_ERRNO(&parser_)) {
//        return false;
//    }

//    count_parsed_ = nparsed;
//    return true;
//}

//void HttpTask::send_complete(const ecron::net::TcpConnectionPtr& conn)
//{
//    if (response_.get_closeconnection())
//        conn->close();
//    else
//        init(conn);
//}

void HttpTask::on_request()
{
    TcpConnectionPtr conn(conn_weak_.lock());
    if (conn)
    {
        http_cb_(this);

        BufferPtr send_buffer = std::make_shared<Buffer>();
        response_.make_buffer(send_buffer.get());
        conn->send(send_buffer);
    }
}

void HttpTask::on_file()
{
    TcpConnectionPtr conn(conn_weak_.lock());
    if (conn)
    {
        BufferPtr buffer = std::make_shared<Buffer>();
        http_body_cb_(this, buffer.get());
        if (buffer->readable_bytes() > 0) {
            conn->send(buffer);
        }
        //else
        //    task->send_complete(conn);
    }
}


/*
void HttpTask::process()
{
    ecron::net::TcpConnectionPtr conn(conn_weak_.lock());
    if (conn)
    {
        // fixme: get current dir
        std::string root_dir = "/home/shared/project/netlib/examples/http/resources";

        response_.code_ = 200;
        response_.path_ = request_.http_url_;

        do_post();

        if (200 == response_.code_) // parse success
        {
            if (response_.path_ == "/")
                response_.path_ += "index";

            // add file type for GET method
            auto it = _Route.find(response_.path_);
            if (it != _Route.end()) {
                response_.path_ += ".html";
            }

            // check file exists(replace by c++17 filesystem)
            response_.path_ = root_dir + response_.path_;
            file_ = fopen(response_.path_.data(), "rb");
            if (nullptr == file_)
                response_.code_ = 404;
            else
            {
                fseek(file_, 0, SEEK_END);
                response_.content_len_ = ftell(file_);
                fseek(file_, 0, SEEK_SET);

                // 403 file access deny
                
                //fclose(pFile);
            }
        }

          // set error html file
    //    auto it = _ErrorCodePath.find(http_code_);
    //    if (it != _ErrorCodePath.end()) {
    //        http_path_ = it->second;
    //    }

        LOG_INFO("response - code: {}, path: {}, size: {}", response_.code_
                    , response_.path_, response_.content_len_);

        if (200 != response_.code_) {
            //resp->init(http_code_, false, http_path_, 0);
            response_.keep_alive_ = false;
        }
        else {
            //resp->init(http_code_, request_.keep_alive_, http_path_, file_size);
            response_.keep_alive_ = request_.keep_alive_;
            is_body_ = true;
        }

        ecron::BufferPtr send_buffer = std::make_shared<ecron::Buffer>();
        response_.make_header(send_buffer.get());
        conn->send(send_buffer);
    }
}*/
/*
void HttpTask::process_body()
{
    ecron::net::TcpConnectionPtr conn(conn_weak_.lock());
    if (conn)
    {
//        if (nullptr == file_) {
//            file_ = fopen(response_.path_.data(), "rb");
//            if (nullptr == file_) {
//                LOG_ERROR("response - open file failed : {}", response_.path_);
//                conn->close();
//                return;
//            }
//        }

        size_t readbytes = 0;
        ecron::BufferPtr buffer = std::make_shared<ecron::Buffer>();
        //buffer->ensure_writable(netlib::Buffer::InitialSize);
        readbytes = fread(buffer->begin_write(), 1, ecron::Buffer::InitialSize, file_);
        if (readbytes < ecron::Buffer::InitialSize)
        {
            //if (feof(file_)) {
            //}
            //else if (ferror(file_)) {
            //}
            fclose(file_);
            file_ = nullptr;

            //LOG_TRACE("response process file complete");
            is_body_ = false;

            // no data to trigger send callback
            // so this assure complete
            if (0 == readbytes)
                send_complete(conn);
        }

        if (readbytes > 0)
        {
            buffer->has_written(readbytes);
            conn->send(buffer);
        }
    }
}*/

//void HttpTask::process_body(const netlib::TcpConnectionPtr& conn)
//{
//    if (200 != http_code_)
//        return;

//    FILE* pFile = fopen(http_path_.data(), "rb");
//    if (NULL == pFile)
//    {
//        //printf("open file error\n");
//        return;
//    }

//    // send num bytes every fread
//    // not control send speed
//    uint32_t readbytes = 0;
//    while (true) {
//        netlib::BufferPtr buffer = std::make_shared<netlib::Buffer>();
//        //buffer->ensure_writable(1024);
//        readbytes = fread(buffer->begin_write(), 1, netlib::Buffer::InitialSize, pFile);
//        if (0 == readbytes)
//            break;
//        buffer->has_written(readbytes);
//        conn->send(buffer);
//    }
//    fclose(pFile);
//}
/*
void HttpTask::do_post()
{
    if (request_.http_method_ != "POST")
        return;

    std::string::size_type n;
    n = request_.http_url_.find("cgi");
    if (n == std::string::npos)
        return;
            
    if (request_.post_.empty()) // not have post message
    {
        response_.code_ = 400;
        return;
    }

    std::string user = request_.post_["user"];
    std::string pwd = request_.post_["password"];
    bool ret = false;
    if (request_.http_url_ == "/login_cgi")
    {
        ret = user_verify(user, pwd, 1);
        if (ret)
            response_.path_ = "/welcome.html";
        else
            response_.path_ = "/login_error.html";
    }
    else if (request_.http_url_ == "/register_cgi")
    {
        ret = user_verify(user, pwd, 0);
        if (ret)
            response_.path_ = "/login.html";
        else
            response_.path_ = "/register_error.html";
    }
    else
    {
        response_.code_ = 400; // not process other cgi post
        return;
    }
    
}*/
/*
bool HttpTask::user_verify(const std::string& name, const std::string& pwd, int login)
{
    LOG_INFO("verify user - name: {}, password: {}, login: {}", 
                                name, pwd, login);

    SqlHandler_t sql;
    SqlConnRAII(&sql, SqlConnPool::Instance());
    if (nullptr == sql)
    {
        LOG_ERROR("database connection error");
        return false;
    }

    std::string query_string;
    MYSQL_RES* res = nullptr;

    query_string = fmt::format("SELECT username, password FROM user WHERE username='{}' LIMIT 1", name);
    if (mysql_query(sql, query_string.c_str())) {
        LOG_ERROR("SELECT query error: {}", query_string);
        return false;
    }

    bool flag = false;
    res = mysql_store_result(sql);
    if (0 == mysql_num_rows(res))
    {
        if (0 == login) // register
        {
            query_string = fmt::format("INSERT INTO user(username, password) VALUES('{}','{}')", name.c_str(), pwd.c_str());
            if (mysql_query(sql, query_string.c_str()))
                LOG_ERROR("INSERT query error: {}", query_string);
            else
                flag = true;
        }
    }
    else
    {
        //num_cols = mysql_num_fields(res);
        //fields = mysql_fetch_fields(res);
        if (0 != login) // login
            while (MYSQL_ROW row = mysql_fetch_row(res))
            {
                //netlib::LOGGER.write_log(netlib::LL_Trace, "MYSQL ROW: {} {}", row[0], row[1]);
                std::string password(row[1]);
                if (pwd == password)
                    flag = true;
                else
                    LOG_ERROR("password auth error");
            }
    }
    mysql_free_result(res);

    if (flag)
        LOG_INFO("verify user OK!");
     else
        LOG_INFO("verify user failed!");
    //SqlConnPool::Instance()->FreeConn(sql);
    return flag;
}*/

