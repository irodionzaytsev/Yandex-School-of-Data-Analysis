#include <vector>
#include <memory>

#include "readers.h"

class LimitReader : public Reader {
private:
    std::unique_ptr<Reader> reader_;
    size_t last_len_;
public:

    LimitReader(std::unique_ptr<Reader> reader, size_t limit) {
        reader_ = std::move(reader);
        last_len_ = limit;
    }

    virtual size_t Read(char* buf, size_t len) override {
        len = std::min(last_len_, len);
        int red_len = reader_->Read(buf, len);
        last_len_ -= red_len;
        return red_len;
    }
};

class TeeReader : public Reader {
private:
    std::vector<std::unique_ptr<Reader>> readers_;
    int current_reader_index_ = 0;

public:
    TeeReader(std::vector<std::unique_ptr<Reader>> readers) {
        for (auto& current_reader : readers) {
            readers_.push_back(std::move(current_reader));
        }
    }

    virtual size_t Read(char* buf, size_t len) override {

        int red_len = 0;

        while(current_reader_index_ < readers_.size() && len > 0) {
            int current_len = readers_[current_reader_index_]->Read(buf, len);
            len -= current_len;
            buf+= current_len;
            red_len += current_len;
            if (len > 0) {
                ++current_reader_index_;
            }
        }
        return red_len;
    }
};

class HexDecodingReader : public Reader {
private:
    std::unique_ptr<Reader> reader_;

public:
    char hexToDigit(char letter) {
        if (letter >= '0' && letter <= '9') {
            return letter - '0';
        } else {
            return letter - 'A' + 10;
        }
    }

    char hexDecode(char* sub_buf) {
        return (hexToDigit(*sub_buf) << 4) | hexToDigit(*(sub_buf + 1));
    }

    HexDecodingReader(std::unique_ptr<Reader> reader) {
        reader_ = std::move(reader);
    }

    virtual size_t Read(char* buf, size_t len) override {
        char sub_buf[2];
        int red_len = 0;
        for (int i = 0; i < len; ++i) {
            size_t current_len = reader_->Read(&sub_buf[0], 2);
            if (current_len < 2) {
                break;
            }
            red_len += 1;
            *buf = hexDecode(&sub_buf[0]);
            ++buf;
        }
        return red_len;
    }
};
