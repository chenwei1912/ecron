#include "Buffer.h"

#include <algorithm>


using namespace netlib;


const size_t Buffer::InitialSize = 1024;


//Buffer::Buffer()
//    : read_index_(0)
//    , write_index_(0)
//{
//memset(recv_buffer_, 0, sizeof(recv_buffer_));
//}

//void Buffer::read()
//{
//}

void Buffer::swap(Buffer& rhs)
{
    buff_.swap(rhs.buff_);
    std::swap(read_index_, rhs.read_index_);
    std::swap(write_index_, rhs.write_index_);
}

void Buffer::write(const char* data, size_t n)
{
    if (NULL == data || 0 == n)
        return;

    ensure_writable(n);

    //memcpy(begin_write(), data, n);
    std::copy(data, data + n, begin_write());
    has_written(n);
}

void Buffer::write(const std::string& str)
{
    write(str.data(), str.size());
}

void Buffer::has_written(size_t n)
{
    if (n <= writable_bytes())
        write_index_ += n;
}

void Buffer::has_readed(size_t n)
{
    //assert(len <= readableBytes());
    if (n < readable_bytes())
        read_index_ += n;
    else
        has_readall();
}

void Buffer::has_readall()
{
    read_index_ = 0;
    write_index_ = 0;
}

std::string Buffer::read_string()
{
    return read_string(readable_bytes());
}

std::string Buffer::read_string(size_t n)
{
    //assert(n <= readableBytes());
    size_t len = readable_bytes();
    if (n > len)
        n = len;

    std::string str(begin_read(), n);
    has_readed(n);
    return str;
}

void Buffer::ensure_writable(size_t n)
{
    if (n > writable_bytes())
        make_space(n);
}

void Buffer::shrink()
{
    buff_.shrink_to_fit(); // cpp11
}

void Buffer::make_space(size_t n)
{
    if (read_index_ + writable_bytes() < n) {
        buff_.resize(write_index_ + n);
    }
    else { // move readable data
        if (0 == read_index_)
            return;

        size_t count = readable_bytes();
        std::copy(begin() + read_index_, 
                    begin() + write_index_, 
                    begin());
        read_index_ = 0;
        write_index_ = count;
    }
}



