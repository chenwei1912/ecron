#include "HttpServer.h"
#include "Logger.h"

#include <boost/asio.hpp>
#include <boost/filesystem.hpp>

#include "SqlConnPool.h"

#include "gflags/gflags.h"

#include <iostream>
#include <regex>


DEFINE_string(host, "0.0.0.0", "IP Address of server");
DEFINE_int32(port, 9876, "TCP Port of remote server");
DEFINE_string(root_dir, "/home/shared/project/ecron/examples/http/resources", "Http server root directory");


static const std::unordered_map<std::string, std::string> _SuffixType = {
    { ".html",  "text/html" },
    { ".xml",   "text/xml" },
    { ".xhtml", "application/xhtml+xml" },
    { ".txt",   "text/plain" },
    { ".rtf",   "application/rtf" },
    { ".pdf",   "application/pdf" },
    { ".word",  "application/nsword" },
    { ".png",   "image/png" },
    { ".gif",   "image/gif" },
    { ".jpg",   "image/jpeg" },
    { ".jpeg",  "image/jpeg" },
    { ".ico",   "image/x-icon"},
    { ".au",    "audio/basic" },
    { ".mpeg",  "video/mpeg" },
    { ".mpg",   "video/mpeg" },
    { ".avi",   "video/x-msvideo" },
    { ".gz",    "application/x-gzip" },
    { ".tar",   "application/x-tar" },
    { ".css",   "text/css "},
    { ".js",    "text/javascript "}
};

static const std::unordered_set<std::string> _Route = {
"/index",
"/login",
"/register",
"/picture",
"/video",
"/fans"
};


static std::string GetFileType(std::string& path)
{
    std::string::size_type idx = path.find_last_of('.');
    if (idx == std::string::npos)
        return "text/plain";

    std::string suffix = path.substr(idx);
    auto it = _SuffixType.find(suffix);
    if (it != _SuffixType.end())
        return it->second;

    return "text/plain";
}

bool parse_postbody(ecron::Buffer* buffer, std::unordered_map<std::string, std::string>& post)
{
    const char* at = buffer->begin_read();
    size_t length = buffer->readable_bytes();

    std::string str_field;
    //std::string str_value;

    //https://stackoverflow.com/questions/32553593/c-regex-extract-all-substrings-using-regex-search
    std::regex pattern("(\\w+)=(\\w+)");
    std::cregex_iterator iter(at, at + length, pattern);
    std::cregex_iterator end;
    for (; iter != end; ++iter)
    {
        for (unsigned i = 1; i < iter->size(); ++i) // i=1 means from first sub-match
        {
            if (str_field.empty())
                str_field = (*iter)[i];
            else
            {
                post[str_field] = (*iter)[i];
                str_field.clear();
            }
        }
    }

    return true;
}

bool user_verify(const std::string& name, const std::string& pwd, int login)
{
    //LOG_INFO("verify user - name: {}, password: {}, login: {}", name, pwd, login);

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

void on_request(ecron::net::HttpTask* task)
{
    std::string path_file;

    ecron::net::HttpRequest* req = task->get_req();
    ecron::net::HttpResponse* resp = task->get_resp();

    do {
        resp->set_code(200);
        resp->set_closeconnection(!req->get_keepalive());

        if ("POST" == req->get_method()) {
            
            //std::string::size_type n;
            //n = request_.http_url_.find("cgi");
            //if (n == std::string::npos)
            //    return;
            std::unordered_map<std::string, std::string> post_map;
            std::string user;
            std::string pwd;
            const std::string* ptype = nullptr;
            ptype = req->get_header("Content-Type");
            if (nullptr != ptype && 0 == ptype->compare("application/x-www-form-urlencoded")) {
                parse_postbody(&(req->get_body()), post_map);
            }
            if (!post_map.empty()) {
                user = post_map.at("user"); // FIXME: not robust
                pwd = post_map.at("password");
            }

            if (req->get_url() == "/login")
            {
                if (user_verify(user, pwd, 1))
                    path_file = "/welcome.html";
                else
                    path_file = "/login_error.html";
            }
            else if (req->get_url() == "/register")
            {
                if (user_verify(user, pwd, 0))
                    path_file = "/login.html";
                else
                    path_file = "/register_error.html";
            }
            else if (req->get_url() == "/picture")
            {
                path_file = "/picture.html";
            }
            else if (req->get_url() == "/video")
            {
                path_file = "/video.html";
            }
            else if (req->get_url() == "/fans")
            {
                path_file = "/fans.html";
            }
            else
            {
                LOG_ERROR("post method not support url: {}", req->get_url());
                resp->set_code(404);
                break;
            }
        }
        else if ("GET" == req->get_method()) {
            path_file = req->get_url();
            if ("/" == path_file)
                path_file += "index";

            // check GET route
            // or check dot extend filetype
            auto it = _Route.find(path_file);
            if (it != _Route.end()) {
                path_file += ".html";
            }
        }
        else {
            LOG_ERROR("http method not support: {}", req->get_method());
            resp->set_code(400);
            break;
        }

        // check file
        if (200 == resp->get_code()) {
            path_file = FLAGS_root_dir + path_file;

            boost::system::error_code ec;
            //If exists(p) && is_regular_file(p)
            std::size_t len = boost::filesystem::file_size(path_file, ec);
            if (ec) {
                LOG_ERROR("file is not exists!");
                resp->set_code(404);
                break;
            }

            FILE* pfile = fopen(path_file.c_str(), "rb");
            if (nullptr == pfile) {
                LOG_ERROR("open file failed: {}", path_file);
                resp->set_code(404);
                break;
            }
            fseek(pfile, 0, SEEK_SET);

            resp->set_content_type(GetFileType(path_file));
            resp->set_content_length(len);

            task->set_send_progressively(true);
            task->set_context(pfile);

            // 403 file access deny

        }
    } while (false);

      // set error html file
//    auto it = _ErrorCodePath.find(http_code_);
//    if (it != _ErrorCodePath.end()) {
//        http_path_ = it->second;
//    }

    if (200 != resp->get_code()) {
        resp->set_closeconnection(true);
    }

    //resp->set_body();

    LOG_INFO("response - code: {}, file: {}", resp->get_code(), path_file);
}

void process_file(ecron::net::HttpTask* task, ecron::Buffer* buffer)
{
    FILE* pfile = reinterpret_cast<FILE*>(task->get_context());
    if (nullptr == pfile)
        return;

    size_t readbytes = 0;
    //buffer->ensure_writable(netlib::Buffer::InitialSize);
    readbytes = fread(buffer->begin_write(), 1, ecron::Buffer::InitialSize, pfile);
    if (readbytes > 0) {
        buffer->has_written(readbytes);
    }
    else {
        //if (feof(file_)) {
        //}
        //else if (ferror(file_)) {
        //}

        //LOG_TRACE("response process file complete");

        fclose(pfile);
        //task->set_send_progressively(false);
        //task->set_context(nullptr);
    }
}


int main(int argc, char* argv[])
{
    // Parse gflags.
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    bool ret = false;
    ret = ecron::LOGGER.init("httpserver.log");
    if (!ret) {
        std::cout << "log init failed." << std::endl;
        return -2;
    }
    ecron::LOGGER.set_level(ecron::LL_Trace);

    SqlConnPool* pool = SqlConnPool::Instance();
    ret = pool->Init("localhost", 3306, "root", "378540", "webserver", 1);
    if (!ret) {
        LOG_ERROR("HttpServer start database failed!");
        return -3;
    }
    
    ecron::net::EventLoop loop;
    ecron::net::HttpServer server(&loop);

    server.set_http_callback(on_request);
    server.set_http_send_callback(process_file);

    ret = server.start(FLAGS_host.c_str(), FLAGS_port, 1, 1);
    if (!ret) {
        std::cout << "http server start failed." << std::endl;
        return -4;
    }

    loop.set_signal_handle([&loop](int signal){
        switch (signal)
        {
            case SIGINT:
            case SIGTERM:
                loop.quit();
                break;
            default:
                break;
        }
    });
    loop.add_signal(SIGINT);
    //loop.remove_signal(SIGINT);

    loop.add_timer(3, []{
        //std::cout << "idle do something." << std::endl;
        ecron::LOGGER.flush();
    }, true);

    loop.loop();

    // never run
    ecron::LOGGER.release();
    std::cout << "main exit." << std::endl;
    return 0;
}



