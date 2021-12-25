// Copyright 2021, Ethan Chen.  All rights reserved.
// Author: Ethan Chen (ChenWei)
//
// This is a public header file, it must only include public header files.

#ifndef ECRON_BUFFER_H
#define ECRON_BUFFER_H


#include <vector>
#include <memory>



namespace ecron
{

class Buffer
{
public:
    static const size_t InitialSize;

    explicit Buffer(size_t initialsize = InitialSize)
                : buff_(initialsize)
                , read_index_(0)
                , write_index_(0)
    {
    }
    //~Buffer();

    // implicit dtor,copy-ctor,move-ctor
    // copy-assignment-operator,move-assignment-operator

    void swap(Buffer& rhs);

    size_t readable_bytes() const { return write_index_ - read_index_; }
    size_t writable_bytes() const { return buff_.size() - write_index_; }

    char* begin_write() { return begin() + write_index_; }
    const char* begin_read() const { return begin() + read_index_; }

    //void read();
    void write(const char* data, size_t n);
    void write(const std::string& str);

    void has_written(size_t n);
    void has_readed(size_t n);
    void has_readall();

    std::string read_string();
    std::string read_string(size_t n);

    void ensure_writable(size_t n);

    void shrink();

private:
    char* begin() { return &buff_[0]; }
    const char* begin() const { return &buff_[0]; }
  
    void make_space(size_t n);

    std::vector<char> buff_;
    size_t read_index_;
    size_t write_index_;
};

typedef std::shared_ptr<Buffer> BufferPtr;

}// namespace ecron

#endif // ECRON_BUFFER_H