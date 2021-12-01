#include "HttpConn.h"
#include "Logger.h"

#include <stdio.h>
#include <cstring>
#include <climits>

#include <unordered_set>
#include <unordered_map>

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

static const std::unordered_map<int, std::string> _CodeStatus = {
    { 200, "OK" },
    { 400, "Bad Request" },
    { 403, "Forbidden" },
    { 404, "Not Found" },
};

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
        : headercomplete_(false)
        , msgcomplete_(false)
{
}

HttpConn::~HttpConn()
{
    //UnmapFile();
}

int HttpConn::init()
{
    http_parser_settings_init(&settings_);
    memset(&settings_, 0, sizeof(settings_));
    settings_.on_message_begin = on_message_begin;
    settings_.on_url = on_url;
    settings_.on_status = on_status;
    settings_.on_header_field = on_header_field;
    settings_.on_header_value = on_header_value;
    settings_.on_headers_complete = on_headers_complete;
    settings_.on_body = on_body;
    settings_.on_message_complete = on_message_complete;
    settings_.on_chunk_header = on_chunk_header;
    settings_.on_chunk_complete = on_chunk_complete;

    http_parser_init(&parser_, HTTP_REQUEST);
    parser_.data = this;

    request_.http_method.clear();
    request_.http_url.clear();
    request_.keep_alive = 0;
    request_.http_headers.clear();
    request_.http_header_field.clear();
    request_.http_body.clear();

    count_parsed_ = 0;
    headercomplete_ = false;
    msgcomplete_ = false;
    post_.clear();
    return 0;
}

bool HttpConn::process(netlib::Buffer* buffer)
{
    size_t nparsed = 0;
    nparsed = http_parser_execute(&parser_, &settings_, buffer->begin_read(), 
                                    buffer->readable_bytes());
    if (HPE_OK != HTTP_PARSER_ERRNO(&parser_)) {
        netlib::LOGGER.write_log(netlib::LL_Error, "HttpServer parse error");

        http_code_ = 400;
    }
    else
    {
        buffer->has_readed(nparsed);
        count_parsed_ += nparsed;
        if (msgcomplete_) {
            netlib::LOGGER.write_log(netlib::LL_Info, "request parse OK! method: {}, url: {}", 
                                        request_.http_method, request_.http_url);
            http_code_ = 200;
            http_path_ = request_.http_url;
        } else {
            netlib::LOGGER.write_log(netlib::LL_Trace, "request parsed {}, need more data", 
                                        count_parsed_);
            return false;
        }
    }

    return true;
}

//void HttpConn::make_response2(netlib::Buffer* buffer)
//{
//    buffer->write("HTTP/1.1 200 OK\r\nConnection: close\r\nContent-type: text/html\r\n");
//    const char* temp = "<html><body><h1>12345</h1></body></html>";
//    size_t len = strlen(temp);
//    buffer->write("Content-length: " + std::to_string(len) + "\r\n\r\n");
//    buffer->write(temp, len);
//}

void HttpConn::make_response(netlib::Buffer* buffer)
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

    //printf("resource file : %s, code : %d\n", (mydir + http_path_).data(), http_code_);
    netlib::LOGGER.write_log(netlib::LL_Info, "response info - code: {}, path: {}", 
                                http_code_, http_path_);

    // error html
    auto it = _ErrorCodePath.find(http_code_);
    if (it != _ErrorCodePath.end()) {
        http_path_ = it->second;
        //stat((srcDir_ + path_).data(), &mmFileStat_);
    }

    //printf("after error file : %s, code : %d\n", (mydir + http_path_).data(), http_code_);

    // add status line
    std::string status;
    auto it_status = _CodeStatus.find(http_code_);
    if (it_status != _CodeStatus.end()) {
        status = it_status->second;
    }
    else {
        http_code_ = 400;
        status = _CodeStatus.find(400)->second;
        // log
    }
    buffer->write("HTTP/1.1 " + std::to_string(http_code_) + " " + status + "\r\n");

    // add header
    buffer->write("Connection: ");
    if ((200 == http_code_) && (0 != request_.keep_alive))
        buffer->write("keep-alive\r\n");
    else
        buffer->write("close\r\n");

    buffer->write("Content-type: " + GetFileType() + "\r\n");

    // add content
    FILE* pFile = fopen((mydir + http_path_).data(), "rb");
    if (NULL == pFile)
    {
        //printf("open file error\n");
        return;
    }

    fseek(pFile, 0, SEEK_END);
    uint32_t FileSize = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);

    buffer->write("Content-length: " + std::to_string(FileSize) + "\r\n\r\n");

    // if size is very large, multi send?
    // partition to send header and body
    uint32_t readbytes = 0;
    while (true) {
        buffer->ensure_writable(512);
        readbytes = fread(buffer->begin_write(), 1, 512, pFile);
        if (0 == readbytes)
            break;
        //printf("file read bytes: %d\n", readbytes);
        buffer->has_written(readbytes);
    }
    fclose(pFile);
}

std::string HttpConn::GetFileType() {
    std::string::size_type idx = http_path_.find_last_of('.');
    if(idx == std::string::npos) {
        return "text/plain";
    }
    std::string suffix = http_path_.substr(idx);
    auto it = _SuffixType.find(suffix);
    if (it != _SuffixType.end())
        return it->second;
    return "text/plain";
}

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

int HttpConn::on_message_begin(http_parser* parser)
{
    //printf("\n***MESSAGE BEGIN***\n\n");
    return 0;
}

int HttpConn::on_url(http_parser* parser, const char* at, size_t length)
{
    HttpConn* wrapper = (HttpConn*)parser->data;
    wrapper->request_.http_url.assign(at, length);

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

int HttpConn::on_status(http_parser* parser, const char* at, size_t length)
{
    //printf("Status: %.*s\n", (int)length, at);
    return 0;
}

int HttpConn::on_header_field(http_parser* parser, const char* at, size_t length)
{
    HttpConn* wrapper = (HttpConn*)parser->data;
    wrapper->request_.http_header_field.assign(at, length);

    //printf("Header field: %.*s\n", (int)length, at);
    return 0;
}

int HttpConn::on_header_value(http_parser* parser, const char* at, size_t length)
{
    HttpConn* wrapper = (HttpConn*)parser->data;    
    wrapper->request_.http_headers[wrapper->request_.http_header_field] = std::string(at, length);

    //printf("Header value: %.*s\n", (int)length, at);
    return 0;
}

int HttpConn::on_headers_complete(http_parser* parser)
{
    HttpConn* wrapper = (HttpConn*)parser->data;
    wrapper->request_.http_method = http_method_str((http_method)parser->method);
    wrapper->request_.keep_alive = http_should_keep_alive(parser);
    wrapper->headercomplete_ = true;

    //printf("\n***HEADERS COMPLETE***\n\n");
    return 0;
}

int HttpConn::on_body(http_parser* parser, const char* at, size_t length)
{
    // this callback function is probably called more than once

    HttpConn* wrapper = (HttpConn*)parser->data;
    wrapper->request_.http_body.append(at, length);

    //printf("Body: %.*s\n", (int)length, at);
    wrapper->parse_post();
    return 0;
}

int HttpConn::on_message_complete(http_parser* parser)
{
    HttpConn* wrapper = (HttpConn*)parser->data;
    wrapper->msgcomplete_ = true;

    //printf("\n***MESSAGE COMPLETE***\n\n");
    return 0;
}

int HttpConn::on_chunk_header(http_parser* parser)
{
    //printf("\n***CHUNK HEADER***\n\n");
    return 0;
}

int HttpConn::on_chunk_complete(http_parser* parser)
{
    //printf("\n***CHUNK COMPLETE***\n\n");
    return 0;
}

