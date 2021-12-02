#include "HttpConn.h"
#include "Logger.h"

#include <stdio.h>
#include <cstring>
#include <climits>

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
"/video"
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

int HttpConn::init()
{
    request_.init();
    http_code_ = 200;
    http_path_.clear();
    return 0;
}

bool HttpConn::process(netlib::Buffer* buffer)
{
    size_t nparsed = 0;
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
//    nparsed = http_parser_execute(&parser_, &settings_, buffer->begin_read(), 
//                                    buffer->readable_bytes());
//    if (HPE_OK != HTTP_PARSER_ERRNO(&parser_)) {
//        netlib::LOGGER.write_log(netlib::LL_Error, "HttpServer parse error");

//        http_code_ = 400;
//    }
//    else
//    {
//        buffer->has_readed(nparsed);
//        count_parsed_ += nparsed;
//        if (msgcomplete_) {
//            netlib::LOGGER.write_log(netlib::LL_Info, "request parse OK! method: {}, url: {}", 
//                                        request_.http_method, request_.http_url);
//            http_code_ = 200;
//            http_path_ = request_.http_url;
//        } else {
//            netlib::LOGGER.write_log(netlib::LL_Trace, "request parsed {}, need more data", 
//                                        count_parsed_);
//            return false;
//        }
//    }
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

    std::string mydir = "/home/shared/project/netlib/output/resources";
    //printf("http_url : %s\n", request_.http_url.data());

    if (200 == http_code_)
    {
        if (http_path_ == "/")
            http_path_ += "index";

        // check route
        auto it = _Route.find(http_path_);
        if (it != _Route.end()) {
            //printf("route find\n");
            http_path_ += ".html";
        }

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
    resp->set_keepalive(request_.keep_alive_);
    resp->make_response(buffer, mydir);

    //printf("after error file : %s, code : %d\n", (mydir + http_path_).data(), http_code_);

    // add status line

//    std::string status;
//    auto it_status = _CodeStatus.find(http_code_);
//    if (it_status != _CodeStatus.end()) {
//        status = it_status->second;
//    }
//    else {
//        http_code_ = 400;
//        status = _CodeStatus.find(400)->second;
//        // log
//    }
//    buffer->write("HTTP/1.1 " + std::to_string(http_code_) + " " + status + "\r\n");

//    // add header
//    buffer->write("Connection: ");
//    if ((200 == http_code_) && (0 != request_.keep_alive))
//        buffer->write("keep-alive\r\n");
//    else
//        buffer->write("close\r\n");

//    buffer->write("Content-type: " + GetFileType() + "\r\n");

//    // add content
//    FILE* pFile = fopen((mydir + http_path_).data(), "rb");
//    if (NULL == pFile)
//    {
//        //printf("open file error\n");
//        return;
//    }

//    fseek(pFile, 0, SEEK_END);
//    uint32_t FileSize = ftell(pFile);
//    fseek(pFile, 0, SEEK_SET);

//    buffer->write("Content-length: " + std::to_string(FileSize) + "\r\n\r\n");

//    // if size is very large, multi send?
//    // partition to send header and body
//    uint32_t readbytes = 0;
//    while (true) {
//        buffer->ensure_writable(512);
//        readbytes = fread(buffer->begin_write(), 1, 512, pFile);
//        if (0 == readbytes)
//            break;
//        //printf("file read bytes: %d\n", readbytes);
//        buffer->has_written(readbytes);
//    }
//    fclose(pFile);
}

//std::string HttpConn::GetFileType() {
//    std::string::size_type idx = http_path_.find_last_of('.');
//    if(idx == std::string::npos) {
//        return "text/plain";
//    }
//    std::string suffix = http_path_.substr(idx);
//    auto it = _SuffixType.find(suffix);
//    if (it != _SuffixType.end())
//        return it->second;
//    return "text/plain";
//}

void HttpConn::parse_post()
{
//    if(request_.http_method == "POST" 
//        && request_.http_headers["Content-Type"] == "application/x-www-form-urlencoded")
//    {
//        ParseFromUrlencoded_();

//        if(DEFAULT_HTML_TAG.count(path_)) {
//            int tag = DEFAULT_HTML_TAG.find(path_)->second;
//            LOG_DEBUG("Tag:%d", tag);
//            if(tag == 0 || tag == 1) {
//                bool isLogin = (tag == 1);
//                if(UserVerify(post_["username"], post_["password"], isLogin)) {
//                    path_ = "/welcome.html";
//                } 
//                else {
//                    path_ = "/error.html";
//                }
//            }
//        }
//    }
}

