#ifndef H_GRID
#define H_GRID

#include <string>
#include <vector>
#include <memory>
#include "general.h"
#include "encoding.h"

template <typename T> using refc = std::shared_ptr<T>;

struct cg_box;
struct cg_other;

struct Grid {
    const int SLIMEBIT_SHIFT = 30, SLIMEBIT = 1 << SLIMEBIT_SHIFT;
    
    refc<cg_box> box;
    refc<cg_other> other;

    Grid(s_i raw, int sep, int just, int fill, bool &empty);
    inline Grid(const Grid &cg) = default;

    int operator()(point p);
    int operator()(int x, int y) { return operator()({ x, y }); }
    bool slime(point p);

    int bWidth();
    int bHeight();

    void set(point p, int ch);
    void setslime(point p, bool slime);
};


#endif
