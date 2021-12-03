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
        resp.init();
        do_request(send_buffer.get(), &resp);

        // send response
        conn->send(send_buffer);

        if (0 == resp.get_keepalive())
            conn->close();

        //http_conn->init();
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

void HttpConn::do_request(netlib::Buffer* buffer, HttpResponse* resp)
{
    // init response
    //UnmapFile();

//    if (request_.http_url == "/")
//        request_.http_url += "index";

    // fixme: get current dir
    std::string mydir = "/home/shared/project/netlib/examples/http/resources";
    //printf("http_url : %s\n", request_.http_url.data());

    if (200 == http_code_) // parse success
    {
        if (http_path_ == "/")
            http_path_ += "index";

        // check route
        auto it = _Route.find(http_path_);
        if (it != _Route.end()) {
            //printf("route find\n");
            http_path_ += ".html";
        }

        std::string::size_type n;
        n = request_.http_url_.find("cgi");
        if (n != std::string::npos && request_.http_method_ == "POST")
            do_post();

        // check file exists
        // replace by c++17 filesystem
        FILE* pFile = fopen((mydir + http_path_).data(), "rb");
        if (NULL == pFile)
            http_code_ = 404;
        else
            fclose(pFile);

        // 403 file access deny
    }
//    do {

//    } while (false);

    // error html
    auto it = _ErrorCodePath.find(http_code_);
    if (it != _ErrorCodePath.end()) {
        http_path_ = it->second;
        //stat((srcDir_ + path_).data(), &mmFileStat_);
    }

    mydir += http_path_;
    //printf("resource file : %s, code : %d\n", (mydir + http_path_).data(), http_code_);
    netlib::LOGGER.write_log(netlib::LL_Info, "response info - code: {}, path: {}", 
                                http_code_, mydir);
    resp->set_code(http_code_);
    bool bval = (http_code_ == 400) ? false : request_.keep_alive_;
    resp->set_keepalive(bval);
    resp->make_response(buffer, mydir);
}

void HttpConn::do_post()
{
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
//    if (name == "" || pwd == "")
//        return false;

    //LOG_INFO("Verify name:%s pwd:%s", name.c_str(), pwd.c_str());

    netlib::LOGGER.write_log(netlib::LL_Info, "verify user - name: {}, password: {}", 
                                name, pwd);

    SqlHandler_t sql;
    SqlConnRAII(&sql, SqlConnPool::Instance());
    if (nullptr == sql)
    {
        netlib::LOGGER.write_log(netlib::LL_Critical, "database connection error");
        return false;
    }
    
    //bool flag = false;
    //uint32_t num_cols = 0;
    //char order[256] = { 0 };
    std::string query_string;
    //MYSQL_FIELD* fields = nullptr;
    MYSQL_RES* res = nullptr;
    
    //if(!isLogin) { flag = true; }

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
            if (mysql_query(sql, query_string.c_str())) {
                netlib::LOGGER.write_log(netlib::LL_Error, "INSERT query error: {}", query_string);
                flag = false;
            }
            else
                flag = true;
            netlib::LOGGER.write_log(netlib::LL_Trace, "register user info {}", flag);
        }
    }
    else
    {
        //num_cols = mysql_num_fields(res);
        //fields = mysql_fetch_fields(res);
        if (0 != login) // login
            while (MYSQL_ROW row = mysql_fetch_row(res))
            {
                netlib::LOGGER.write_log(netlib::LL_Trace, "MYSQL ROW: {} {}", row[0], row[1]);
                std::string password(row[1]);
                if (pwd == password)
                {
                    flag = true;
                    netlib::LOGGER.write_log(netlib::LL_Trace, "password match");
                    break;
                }
                netlib::LOGGER.write_log(netlib::LL_Trace, "password error");
            }
    }
    mysql_free_result(res);

    //SqlConnPool::Instance()->FreeConn(sql);
    if (flag)
        netlib::LOGGER.write_log(netlib::LL_Info, "verify user success!");
    //LOG_DEBUG( "UserVerify success!!");
    return flag;
}

