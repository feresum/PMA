#ifndef H_GENERAL
#define H_GENERAL

#include <algorithm>
#include <cassert>
#include <vector>

using std::max;
using std::min;

using std::vector;

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

#define OUT_CHAR ((int)0x80000000)

#define NEVERHAPPEN assert(0),0;


enum match_result {
    MATCH_RESULT_FAIL,
    MATCH_RESULT_SUCCESS,
    MATCH_RESULT_OVERFLOW
};
#define MAX_PATTERN_STACK 99999

template<typename Sub, typename Super> inline bool isA(Super *thing) {
    return !!dynamic_cast<Sub*>(thing);
}

template<typename T> typename vector<T>::iterator operator+(vector<T> &v, int i) {
    return v.begin() + i;
}


#endif