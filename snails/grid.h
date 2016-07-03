#ifndef H_GRID
#define H_GRID

#include <string>
#include <vector>
#include <memory>
#include "general.h"
#include "encoding.h"

template <typename T>
struct refc {
    T* ptr;
    int* count;
    refc(T* p): ptr(p), count(new int(1)) { }
    int use_count() { return *count; }
    refc(const refc& o): ptr(o.ptr), count(o.count) {
        ++*count;
    }
    void decrement() {
        if (*count == 1) {
            delete count;
            delete ptr;
        } else {
            --*count;
        }
    }
    ~refc() { decrement(); }
    void reset(T* p) {
        decrement();
        ptr = p;
        count = new int(1);
    }
    T& operator*() { return *ptr; }
    T* operator->() { return ptr; }
};

struct cg_box;
struct cg_other;

struct Grid {
    const int SLIMEBIT_SHIFT = 30, SLIMEBIT = 1 << SLIMEBIT_SHIFT;
    
    refc<cg_box> box;
    refc<cg_other> other;

    Grid(s_i raw, int sep, int just, int fill, bool &empty);
    Grid(const Grid &cg);
    ~Grid();

    int operator()(point p);
    int operator()(int x, int y) { return operator()({ x, y }); }
    bool slime(point p);

    int bWidth();
    int bHeight();

    void set(point p, int ch);
    void setslime(point p, bool slime);
};


#endif
