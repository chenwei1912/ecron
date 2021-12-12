#include "catch.hpp"

#include "Buffer.h"


#include <iostream>


TEST_CASE("test Buffer read/write", "[BufferReadWrite]") {
    netlib::Buffer buffer;

    REQUIRE(0 == buffer.readable_bytes());
    REQUIRE(netlib::Buffer::InitialSize == buffer.writable_bytes());

    const std::string str("hello");
    buffer.write(str);
    REQUIRE(str.size() == buffer.readable_bytes());
    REQUIRE(netlib::Buffer::InitialSize - str.size() == buffer.writable_bytes());

    std::string str2;
    str2.assign(buffer.begin_read(), 3);
    buffer.has_readed(3);
    REQUIRE(3 == str2.size());
    REQUIRE(2 == buffer.readable_bytes());
    REQUIRE(netlib::Buffer::InitialSize - str.size() == buffer.writable_bytes());
    REQUIRE("hel" == str2);

    buffer.write(str);
    REQUIRE(2 * str.size() - str2.size() == buffer.readable_bytes());
    REQUIRE(netlib::Buffer::InitialSize - 2 * str.size() == buffer.writable_bytes());

    std::string str3;
    str3.assign(buffer.begin_read(), buffer.readable_bytes());
    buffer.has_readed(buffer.readable_bytes());
    REQUIRE(7 == str3.size());
    REQUIRE(0 == buffer.readable_bytes());
    REQUIRE(netlib::Buffer::InitialSize == buffer.writable_bytes());
    REQUIRE("lohello" == str3);
    
    //CHECK();
    //REQUIRE_FALSE();

}

TEST_CASE("test buffer grow", "[BufferGrow]") {
    netlib::Buffer buffer;

    std::string str;
    str.assign(400, 'y');
    buffer.write(str);
    REQUIRE(400 == buffer.readable_bytes());
    REQUIRE(netlib::Buffer::InitialSize - 400 == buffer.writable_bytes());

    buffer.has_readed(50);
    REQUIRE(350 == buffer.readable_bytes());
    REQUIRE(netlib::Buffer::InitialSize - 400 == buffer.writable_bytes());
    
    //std::string str;
    str.assign(1000, 'z');
    buffer.write(str);
    REQUIRE(1350 == buffer.readable_bytes());
    REQUIRE(0 == buffer.writable_bytes());

    buffer.has_readed(buffer.readable_bytes());
    REQUIRE(0 == buffer.readable_bytes());
    REQUIRE(1400 == buffer.writable_bytes());
}

TEST_CASE("test buffer inside grow", "[BufferInsideGrow]") {
    netlib::Buffer buffer;

    std::string str;
    str.assign(800, 'y');
    buffer.write(str);
    REQUIRE(800 == buffer.readable_bytes());
    REQUIRE(netlib::Buffer::InitialSize - 800 == buffer.writable_bytes());

    buffer.has_readed(500);
    REQUIRE(300 == buffer.readable_bytes());
    REQUIRE(netlib::Buffer::InitialSize - 800 == buffer.writable_bytes());
    
    //std::string str;
    str.assign(300, 'z');
    buffer.write(str);
    REQUIRE(600 == buffer.readable_bytes());
    REQUIRE(netlib::Buffer::InitialSize - 600 == buffer.writable_bytes());

}

TEST_CASE("test buffer shrink", "[BufferShrink]") {
    netlib::Buffer buffer;

    std::string str;
    str.assign(2000, 'y');
    buffer.write(str);
    REQUIRE(2000 == buffer.readable_bytes());
    REQUIRE(0 == buffer.writable_bytes());

    buffer.has_readed(1500);
    REQUIRE(500 == buffer.readable_bytes());
    REQUIRE(0 == buffer.writable_bytes());

    buffer.shrink();
    REQUIRE(500 == buffer.readable_bytes());
    REQUIRE(0 == buffer.writable_bytes());

    str.assign(500, 'y');
    REQUIRE(str == buffer.read_string());

}

TEST_CASE("test buffer move", "[BufferMove]") {

    netlib::Buffer buffer;
    buffer.write("netlibx", 7);
    const char* inner = buffer.begin_read();
    
    netlib::Buffer newbuf(std::move(buffer));
    REQUIRE(inner == newbuf.begin_read());

}

