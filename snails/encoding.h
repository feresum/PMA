#ifndef H_ENCODING
#define H_ENCODING

#include <string>
#include "general.h"

enum encoding_t {
    ENC_ASCII,
    ENC_UTF8,
    ENC_SINGLE_BYTE
};

int read_UTF8_ch(const char** s);

int read_UTF8_ch(const char* s);

encoding_t detect_encoding(const str& s);

struct s_i {
    const str &s;
    bool utf8;
    const char* i;

    s_i(const str &s, bool utf8, size_t off = 0) : s(s), utf8(utf8), i(s.c_str() + off) {}

    bool atEnd() {
        return i >= s.c_str() + s.size();
    }
    size_t ind() {
        return i - s.c_str();
    }

    int get() {
        if (atEnd())
            return -1;
        return utf8 ? read_UTF8_ch(&i) : *i++;
    }
    int peek(size_t n = 0) {
        if (utf8) {
            const char* p = i;
            while (n--) {
                if (atEnd())
                    return -1;
                read_UTF8_ch(&p);
            }
            if (atEnd())
                return -1;
            return read_UTF8_ch(p);
        } else {
            if (n + ind() >= s.size()) return -1;
            return i[n];
        }
    }
    void back() {
        if (utf8) {
            while ((*i-- & 0xC0) == 0x80);
        } else {
            --i;
        }
    }
};

#endif
