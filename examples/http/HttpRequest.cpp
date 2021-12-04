#include "HttpRequest.h"
//#include "Logger.h"

#include <stdio.h>
#include <cstring>
//#include <climits>

#include <regex>




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

static int on_message_begin(http_parser* parser)
{
    //printf("\n***MESSAGE BEGIN***\n\n");
    return 0;
}

static int on_url(http_parser* parser, const char* at, size_t length)
{
    HttpRequest* req = (HttpRequest*)parser->data;
    req->http_url_.assign(at, length);

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

static int on_status(http_parser* parser, const char* at, size_t length)
{
    //printf("Status: %.*s\n", (int)length, at);
    return 0;
}

static int on_header_field(http_parser* parser, const char* at, size_t length)
{
    HttpRequest* req = (HttpRequest*)parser->data;
    req->http_header_field_.assign(at, length);

    //printf("Header field: %.*s\n", (int)length, at);
    return 0;
}

static int on_header_value(http_parser* parser, const char* at, size_t length)
{
    HttpRequest* req = (HttpRequest*)parser->data;
    req->http_headers_[req->http_header_field_] = std::string(at, length);

    //printf("Header value: %.*s\n", (int)length, at);
    return 0;
}

static int on_headers_complete(http_parser* parser)
{
    HttpRequest* req = (HttpRequest*)parser->data;
    req->http_method_ = http_method_str((http_method)parser->method);
    req->http_method_ = http_method_str((http_method)parser->method);
    if (0 == http_should_keep_alive(parser))
        req->keep_alive_ = false;
    else
        req->keep_alive_ = true;
    req->headercomplete_ = true;

    //printf("\n***HEADERS COMPLETE***\n\n");
    return 0;
}

static int on_body(http_parser* parser, const char* at, size_t length)
{
    // this callback function is probably called more than once

    HttpRequest* req = (HttpRequest*)parser->data;
    //req->http_body_.append(at, length);

    //printf("Body: %.*s\n", (int)length, at);

    req->parse_post(at, length);
    return 0;
}

static int on_message_complete(http_parser* parser)
{
    HttpRequest* req = (HttpRequest*)parser->data;
    req->msgcomplete_ = true;

    //printf("\n***MESSAGE COMPLETE***\n\n");
    return 0;
}

static int on_chunk_header(http_parser* parser)
{
    //printf("\n***CHUNK HEADER***\n\n");
    return 0;
}

static int on_chunk_complete(http_parser* parser)
{
    //printf("\n***CHUNK COMPLETE***\n\n");
    return 0;
}


HttpRequest::HttpRequest()
        : headercomplete_(false)
        , msgcomplete_(false)
{
}

HttpRequest::~HttpRequest()
{
    //UnmapFile();
}

void HttpRequest::init()
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

    http_method_.clear();
    http_url_.clear();
    //http_version_.clear();
    keep_alive_ = false;
    http_headers_.clear();
    http_header_field_.clear();
    http_body_.clear();

    headercomplete_ = false;
    msgcomplete_ = false;
    count_parsed_ = 0;
    post_.clear();
}

bool HttpRequest::parse(const char* pdata, uint32_t len)
{
    size_t nparsed = 0;
    nparsed = http_parser_execute(&parser_, &settings_, pdata, len);
    if (HPE_OK != HTTP_PARSER_ERRNO(&parser_)) {
        //netlib::LOGGER.write_log(netlib::LL_Error, "HttpServer parse error");
        return false;
    }

    count_parsed_ += nparsed;
    return true;
}

bool HttpRequest::parse_post(const char* at, size_t length)
{
    if (http_method_ == "POST" 
        && http_headers_["Content-Type"] == "application/x-www-form-urlencoded")
    {
        std::string str_field;
        //std::string str_value;

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
                    post_[str_field] = (*iter)[i];
                    str_field.clear();
                }
            }
        }
    }

    return true;
}
