#include "encoding.h"

int read_UTF8_ch(const char** st) {
    const char*& s = *st;
    if ((*s & 0x80) == 0)
        return *s++;
    if ((*s & 0xE0) == 0xC0) {
        int ch = (*s++ & 0x1F) << 6;
        return ch | (*s++ & 0x3F);
    }
    if ((*s & 0xF0) == 0xE0) {
        int ch = (*s++ & 0x0F) << 12;
        ch |= (*s++ & 0x3F) << 6;
        return ch | (*s++ & 0x3F);
    }
    int ch = (*s++ & 0x07) << 18;
    ch |= (*s++ & 0x3F) << 12;
    ch |= (*s++ & 0x3F) << 6;
    return ch | (*s++ & 0x3F);
}

int read_UTF8_ch(const char* s) {
    return read_UTF8_ch(&s);
}


encoding_t detect_encoding(const str& s) {
    bool anyhigh = false;
    for (size_t i = 0; i < s.size();) {
        if ((s[i] & 0x80) == 0) {
            i += 1;
            continue;
        }
        anyhigh = true;
        if ((s[i] & 0xE0) == 0xC0 && (s[i + 1] & 0xC0) == 0x80) {
            if (read_UTF8_ch(&s[i]) < 0x80)
                return ENC_SINGLE_BYTE;
            i += 2;
        } else if ((s[i] & 0xF0) == 0xE0 && (s[i + 1] & 0xC0) == 0x80 && (s[i + 2] & 0xC0) == 0x80) {
            if (read_UTF8_ch(&s[i]) < 0x800)
                return ENC_SINGLE_BYTE;
            i += 3;
        } else if ((s[i] & 0xF8) == 0xF0 && (s[i + 1] & 0xC0) == 0x80 && (s[i + 2] & 0xC0) == 0x80 && (s[i + 3] & 0xC0) == 0x80) {
            int c = read_UTF8_ch(&s[i]);
            if (c < 0x10000 || c > 0x10FFFF)
                return ENC_SINGLE_BYTE;
            i += 4;
        } else {
            return ENC_SINGLE_BYTE;
        }
    }
    return anyhigh ? ENC_UTF8 : ENC_ASCII;
}
