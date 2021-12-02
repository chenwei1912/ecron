#ifndef _HTTP_RESPONSE_H_
#define _HTTP_RESPONSE_H_

#include "http_parser.h"
#include "Buffer.h"

#include <unordered_map>
#include <memory>

//#include <unistd.h>      // close
//#include <sys/stat.h>    // stat
//#include <sys/mman.h>    // mmap, munmap

//enum HttpStatusCode
//{
//    kUnknown,
//    k200Ok = 200,
//    k301MovedPermanently = 301,
//    k400BadRequest = 400,
//    k404NotFound = 404,
//};
  
struct HttpResponse
{
    HttpResponse();
    ~HttpResponse();

    void init();

    void set_code(int code);
    void set_keepalive(bool on);
    bool get_keepalive() { return keep_alive_; }
    
    void make_response(netlib::Buffer* buffer, std::string& path);
    //void make_response2(netlib::Buffer* buffer);
    //std::string GetFileType(std::string& path);

    int http_code_;
    bool keep_alive_;
    std::unordered_map<std::string, std::string> http_headers_;
    std::string http_body_;
};

#endif // _HTTP_RESPONSE_H_

