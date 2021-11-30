#include "Buffer.h"

#include <algorithm>

//#include <string.h>


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
    read_index_ += n;
    if (read_index_ == write_index_) {
        read_index_ = 0;
        write_index_ = 0;
    }
}

void Buffer::make_space(size_t n)
{
    if (read_index_ + writable_bytes() < n) {
        buff_.resize(write_index_ + n);
    }
    else { // move readabel data
        if (0 == read_index_)
            return;

        size_t count = readable_bytes();
        std::copy(buff_.begin() + read_index_, 
                    buff_.begin() + write_index_, 
                    buff_.begin());
        read_index_ = 0;
        write_index_ = count;
    }
}

void Buffer::ensure_writable(size_t n)
{
    if (n > writable_bytes())
        make_space(n);
}

