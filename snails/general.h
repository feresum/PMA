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



#define OUT_CHAR ((int)0x80000000)

#define NEVERHAPPEN assert(0),0;


#define MAX_PATTERN_STACK 99999

template<typename Sub, typename Super> inline bool isA(Super *thing) {
    return !!dynamic_cast<Sub*>(thing);
}

template<typename T> typename vector<T>::iterator operator+(vector<T> &v, int i) {
    return v.begin() + i;
}

const point DIRECTION_LIST[] = { { 1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 } };


#endif