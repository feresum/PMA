#include <string>
#include <vector>
#include <memory>
#include "general.h"

template <typename T> using refc = std::shared_ptr<T>;

struct cg_box;
struct cg_other;

struct Grid {
    refc<cg_box> box;
    refc<cg_other> other;
    std::vector<bool> bslime;

    Grid(std::string &raw, int sep, int just, int fill);
    Grid(Grid &cg);

    int operator()(point p);
    int operator()(int x, int y) { return operator()({ x, y }); }
    bool slime(point p);

    void set(point p, int ch);
    void setslime(point p, bool slime);
};

