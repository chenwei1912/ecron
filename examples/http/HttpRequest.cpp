#include "HttpRequest.h"
//#include "Logger.h"
#include "spdlog/fmt/fmt.h"


using namespace ecron::net;


HttpRequest::HttpRequest()
        //: headercomplete_(false)
        //, msgcomplete_(false)
{
}

HttpRequest::~HttpRequest()
{
    //UnmapFile();
}

void HttpRequest::init()
{
    method_.clear();
    url_.clear();
    //version_.clear();
    keep_alive_ = false;
    headers_.clear();
    body_.has_readall();
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

//bool HttpRequest::parse_postbody()
//{
//    if (http_method_ == "POST" 
//        && http_headers_["Content-Type"] == "application/x-www-form-urlencoded")
//    {
//        const char* at = http_body_.data();
//        size_t length = http_body_.size();

//        std::string str_field;
//        //std::string str_value;

//        //https://stackoverflow.com/questions/32553593/c-regex-extract-all-substrings-using-regex-search
//        std::regex pattern("(\\w+)=(\\w+)");
//        std::cregex_iterator iter(at, at + length, pattern);
//        std::cregex_iterator end;
//        for (; iter != end; ++iter)
//        {
//            for (unsigned i = 1; i < iter->size(); ++i) // i=1 means from first sub-match
//            {
//                if (str_field.empty())
//                    str_field = (*iter)[i];
//                else
//                {
//                    post_[str_field] = (*iter)[i];
//                    str_field.clear();
//                }
//            }
//        }
//    }

//    return true;
//}
