#ifndef H_GENERAL
#define H_GENERAL

#include <stddef.h>
#include <algorithm>
#include <cassert>
#include <vector>
#include <string>

using std::max;
using std::min;

using std::vector;

typedef std::string str;

struct point { int x, y; };
inline bool operator==(point a, point b) { return a.x == b.x && a.y == b.y; }
inline point operator+(point a, point b) { return { a.x + b.x, a.y + b.y }; }



#define OUT_CHAR ((int)0x80000000)

#if defined(_MSC_VER)
#define NEVERHAPPEN { assert(!"This is not supposed to happen"); __assume(false); }
#else
#define NEVERHAPPEN { assert(!"This is not supposed to happen"); __builtin_unreachable(); }
#endif


#define MAX_PATTERN_STACK 99999

template<typename Sub, typename Super> inline bool isA(Super *thing) {
    return !!dynamic_cast<Sub*>(thing);
}

template<typename T> typename vector<T>::iterator operator+(vector<T> &v, int i) {
    return v.begin() + i;
}

template<typename T> T pop(vector<T>& v) {
    T e = std::move(v.back());
    v.pop_back();
    return e;
}

const point DIRECTION_LIST[] = { { 1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 } };


#endif