#ifndef H_GENERAL
#define H_GENERAL

#include <algorithm>
#include <cassert>
#include <vector>

using std::max;
using std::min;

typedef std::string str;

struct point { int x, y; };
inline bool operator==(point a, point b) { return a.x == b.x && a.y == b.y; }
inline point operator+(point a, point b) { return { a.x + b.x, a.y + b.y }; }


enum m_start {
    M_START_TOPLEFT,
    M_START_ALLBOX
};

enum m_type {
    M_TYPE_COUNT,
    M_TYPE_BOOLEAN
};

enum m_just {
    M_JUST_LEFT,
    M_JUST_RIGHT
};

#endif