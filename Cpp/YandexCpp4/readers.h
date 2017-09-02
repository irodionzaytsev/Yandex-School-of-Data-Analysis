#pragma once

#include <string>
#include <algorithm>

#include <unistd.h>

class Reader {
 public:
    virtual ~Reader() {}

    // Читает не более чем @len байт из потока.
    // Помещает результат чтения в буффер @buf.
    // Возвращает, сколько байт удалось прочитать.
    // Возвращаемое значение 0 означает конец потока.

    virtual size_t Read(char* buf, size_t len) = 0;
};

std::string ReadAll(Reader* in) {
    const size_t CHUNK_SIZE = 128;

    std::string buf;
    std::string chunk;
    while (true) {
        chunk.resize(CHUNK_SIZE);
        size_t read_res = in->Read(&(chunk[0]), chunk.size());
        if (read_res == 0) break;

        chunk.resize(read_res);
        buf += chunk;
    }

    return buf;
}

class StringReader : public Reader {
 public:
    StringReader(const std::string& data) : data_(data) {}

    virtual size_t Read(char* buf, size_t len) override {
        size_t read_len = std::min(len, data_.size() - pos_);
        for (size_t i = 0; i < read_len; ++i) {
            buf[i] = data_[pos_++];
        }
        return read_len;
    }

 private:
    std::string data_;
    size_t pos_ = 0;
};

class FdReader : public Reader {
 public:
    FdReader(int fd) : fd_(fd) {}

    virtual size_t Read(char* buf, size_t len) override {
        ssize_t res = ::read(fd_, buf, len);
        if (res == -1) { // treat errors as EOF
            res = 0;
        }
        return res;
    }

 private:
    int fd_;
};
