#include "HttpConn.h"
#include "Logger.h"
#include "SqlConnPool.h"

//#include <cstring>
//#include <climits>

#include <unordered_set>
#include <unordered_map>

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

static const std::unordered_map<int, std::string> _ErrorCodePath = {
    { 400, "/400.html" },
    { 403, "/403.html" },
    { 404, "/404.html" },
};

static const std::unordered_set<std::string> _Route = {
"/index",
"/login",
"/register",
"/picture",
"/video",
"/fans"
};


HttpConn::HttpConn()
{
}

HttpConn::~HttpConn()
{
    //LOG_TRACE("HttpConn destructing");
    if (nullptr != file_)
        fclose(file_);
}

void HttpConn::init(const ecron::net::TcpConnectionPtr& conn)
{
    conn_weak_ = conn;
    request_.init();
    response_.init();
    is_body_ = false;
    file_ = nullptr;
}

bool HttpConn::parse(ecron::Buffer* buffer)
{
    if (!request_.parse(buffer->begin_read(), buffer->readable_bytes()))
    {
        //buffer->has_readed(buffer->readable_bytes());
        LOG_ERROR("parse http protocol error");
        response_.code_ = 400;
        return false;
    }

    buffer->has_readed(request_.count_parsed_);
    if (request_.msgcomplete_) {
        LOG_INFO("request parse OK! method: {}, url: {}", 
                                    request_.http_method_, request_.http_url_);

    } else {
        LOG_TRACE("request parsed {}, need more data", 
                                    request_.count_parsed_);
    }

    return true;
}

void HttpConn::send_complete(const ecron::net::TcpConnectionPtr& conn)
{
    if (response_.keep_alive_)
        init(conn);
    else
        conn->close();
}

void HttpConn::process()
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
}

void HttpConn::process_body()
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
}

//void HttpConn::process_body(const netlib::TcpConnectionPtr& conn)
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

void HttpConn::do_post()
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
    
}

bool HttpConn::user_verify(const std::string& name, const std::string& pwd, int login)
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
}

