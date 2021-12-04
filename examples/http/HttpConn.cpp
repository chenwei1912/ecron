#include "HttpConn.h"
#include "Logger.h"
#include "SqlConnPool.h"

#include <stdio.h>
#include <cstring>
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
//        : headercomplete_(false)
//        , msgcomplete_(false)
{
}

HttpConn::~HttpConn()
{
    //UnmapFile();
}

int HttpConn::init(const netlib::TcpConnectionPtr& conn)
{
    request_.init();
    http_code_ = 200;
    http_path_.clear();

    conn_weak_ = conn;
    return 0;
}

bool HttpConn::parse(netlib::Buffer* buffer)
{
    bool ret = request_.parse(buffer->begin_read(), buffer->readable_bytes());
    if (!ret) {
        netlib::LOGGER.write_log(netlib::LL_Error, "HttpServer parse error");
        http_code_ = 400;
    }
    else
    {
        buffer->has_readed(request_.count_parsed_);
        if (request_.msgcomplete_) {
            netlib::LOGGER.write_log(netlib::LL_Info, "request parse OK! method: {}, url: {}", 
                                        request_.http_method_, request_.http_url_);
            http_code_ = 200;
            http_path_ = request_.http_url_;
        } else {
            netlib::LOGGER.write_log(netlib::LL_Trace, "request parsed {}, need more data", 
                                        request_.count_parsed_);
            return false;
        }
    }

    return true;
}

void HttpConn::process()
{
    netlib::TcpConnectionPtr conn(conn_weak_.lock());
    if (conn)
    {
        netlib::BufferPtr send_buffer = std::make_shared<netlib::Buffer>();
        HttpResponse resp;
        process_header(send_buffer.get(), &resp);
        conn->send(send_buffer);

        process_body(conn);

        if (!resp.get_keepalive())
            conn->close();
    }
}

//void HttpConn::make_response2(netlib::Buffer* buffer)
//{
//    buffer->write("HTTP/1.1 200 OK\r\nConnection: close\r\nContent-type: text/html\r\n");
//    const char* temp = "<html><body><h1>12345</h1></body></html>";
//    size_t len = strlen(temp);
//    buffer->write("Content-length: " + std::to_string(len) + "\r\n\r\n");
//    buffer->write(temp, len);
//}

void HttpConn::process_header(netlib::Buffer* buffer, HttpResponse* resp)
{
    // fixme: get current dir
    std::string mydir = "/home/shared/project/netlib/examples/http/resources";
    //printf("http_url : %s\n", request_.http_url.data());
    uint32_t file_size = 0;

    do_post();

    if (200 == http_code_) // parse success
    {
        if (http_path_ == "/")
            http_path_ += "index";

        // add file type for GET method
        auto it = _Route.find(http_path_);
        if (it != _Route.end()) {
            //printf("route find\n");
            http_path_ += ".html";
        }

        // check file exists(replace by c++17 filesystem)
        FILE* pFile = fopen((mydir + http_path_).data(), "rb");
        if (NULL == pFile)
            http_code_ = 404;
        else
        {
            fseek(pFile, 0, SEEK_END);
            file_size = ftell(pFile);
            fseek(pFile, 0, SEEK_SET);

            // 403 file access deny
            
            fclose(pFile);
        }
    }

      // set error html file
//    auto it = _ErrorCodePath.find(http_code_);
//    if (it != _ErrorCodePath.end()) {
//        http_path_ = it->second;
//    }

    http_path_ = mydir + http_path_;
    netlib::LOGGER.write_log(netlib::LL_Info, "response info - code: {}, path: {}, size: {}", 
                                http_code_, http_path_, file_size);

    if (200 != http_code_)
        resp->init(http_code_, false, http_path_, 0);
    else
        resp->init(http_code_, request_.keep_alive_, http_path_, file_size);
    resp->make_header(buffer);
}

void HttpConn::process_body(const netlib::TcpConnectionPtr& conn)
{
    if (200 != http_code_)
        return;

    FILE* pFile = fopen(http_path_.data(), "rb");
    if (NULL == pFile)
    {
        //printf("open file error\n");
        return;
    }

    // send num bytes every fread
    // not control send speed
    uint32_t readbytes = 0;
    while (true) {
        netlib::BufferPtr buffer = std::make_shared<netlib::Buffer>();
        //buffer->ensure_writable(1024);
        readbytes = fread(buffer->begin_write(), 1, netlib::Buffer::InitialSize, pFile);
        if (0 == readbytes)
            break;
        buffer->has_written(readbytes);
        conn->send(buffer);
    }
    fclose(pFile);
}

void HttpConn::do_post()
{
    if (request_.http_method_ != "POST")
        return;

    std::string::size_type n;
    n = request_.http_url_.find("cgi");
    if (n == std::string::npos)
        return;
            
    if (request_.post_.empty())
    {
        http_code_ = 400;
        return;
    }

    std::string user = request_.post_["user"];
    std::string pwd = request_.post_["password"];
    bool ret = false;
    if (request_.http_url_ == "/login_cgi")
    {
        ret = user_verify(user, pwd, 1);
        if (ret)
            http_path_ = "/welcome.html";
        else
            http_path_ = "/login_error.html";
    }
    else if (request_.http_url_ == "/register_cgi")
    {
        ret = user_verify(user, pwd, 0);
        if (ret)
            http_path_ = "/login.html";
        else
            http_path_ = "/register_error.html";
    }
    else
    {
        http_code_ = 400; // not process other cgi post
        return;
    }
    
}

bool HttpConn::user_verify(const std::string& name, const std::string& pwd, int login)
{
    netlib::LOGGER.write_log(netlib::LL_Info, "verify user - name: {}, password: {}, login: {}", 
                                name, pwd, login);

    SqlHandler_t sql;
    SqlConnRAII(&sql, SqlConnPool::Instance());
    if (nullptr == sql)
    {
        netlib::LOGGER.write_log(netlib::LL_Error, "database connection error");
        return false;
    }

    std::string query_string;
    MYSQL_RES* res = nullptr;

    query_string = fmt::format("SELECT username, password FROM user WHERE username='{}' LIMIT 1", name);
    if (mysql_query(sql, query_string.c_str())) {
        netlib::LOGGER.write_log(netlib::LL_Error, "SELECT query error: {}", query_string);
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
                netlib::LOGGER.write_log(netlib::LL_Error, "INSERT query error: {}", query_string);
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
                    netlib::LOGGER.write_log(netlib::LL_Error, "password auth error");
            }
    }
    mysql_free_result(res);

    if (flag)
        netlib::LOGGER.write_log(netlib::LL_Info, "verify user OK!");
     else
        netlib::LOGGER.write_log(netlib::LL_Info, "verify user failed!");
    //SqlConnPool::Instance()->FreeConn(sql);
    return flag;
}

