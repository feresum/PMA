#include "grid.h"

struct cg_box {
    int w, h;
    int *data;

    cg_box(const std::string &raw, int sep, int just, int fill) {
        int maxrow = 0, nrows = 0, rlen = 0;
        for (auto c : raw) {
            if (c == sep) {
                maxrow = max(maxrow, rlen);
                rlen = 0;
                nrows++;
            } else {
                rlen++;
            }
        }
        if (rlen) {
            maxrow = max(maxrow, rlen);
            nrows++;
        }
        bool empty = maxrow < 1 && nrows < 1;
        if (empty) fill = OUT_CHAR;
        w = max(1, maxrow);
        h = max(1, nrows);
        data = new int[w*h];
        for (int i = 0; i < w*h; i++) data[i] = fill;

        int row = 0, col = 0;
        for (auto c : raw) {
            if (c == sep) {
                col = 0;
                row++;
            } else {
                data[w*row + (just == 1 ? w - 1 - col : col)] = c;
                col++;
            }
        }
    }

    cg_box(const cg_box &o): w(o.w), h(o.h) {
        data = new int[w*h];
        memcpy(data, o.data, w*h*sizeof(int));
    }

    ~cg_box() {
        delete[] data;
    }

    int& at(point p) {
        assert(contains(p));
        return data[p.y*w + p.x];
    }

    bool contains(point p) {
        return p.x >= 0 && p.x < w && p.y >= 0 && p.y < h;
    }

    void set(point p, int ch) {
        assert(contains(p));
        data[p.y*w + p.x] = ch;
    }
};

struct cg_other {
    struct cgo_char { point p; int ch; };
    vector<cgo_char> data;

    int operator()(point p);
    void set(point p, int ch);
};

template<typename T> inline void uniquify(refc<T> &ptr) {
    if (ptr.use_count() > 1) {
        ptr.reset(new T{ *ptr });
    }
}


int cg_other::operator()(point p) {
    for (unsigned i = 0; i < data.size(); i++) {
        if (data[i].p == p) return data[i].ch;
    }
    return -1;
}

void cg_other::set(point p, int ch) { //TODO
    for (unsigned i = 0; i < data.size(); i++) {
        if (data[i].p == p) {
            data[i].ch = ch;
            return;
        }
    }
    data.push_back({ p, ch });
}

Grid::Grid(const std::string &raw, int sep, int just, int fill):
        box(new cg_box(raw,sep,just,fill)),
        other(new cg_other) { }

int Grid::operator()(point p) {
    if (box->contains(p)) {
        return box->at(p);
    }
    return (*other)(p);
}
    
void Grid::set(point p, int ch) {
    if (box->contains(p)) box->set(p, ch);
    else other->set(p, ch);
}

bool Grid::slime(point p) {
    if (box->contains(p)) return !!(box->at(p) & SLIMEBIT);
    for (auto &d: other->data) {
        if (d.p == p) return !!(d.ch & SLIMEBIT);
    }
    return false;
}

void Grid::setslime(point p, bool slime) {
    if (box->contains(p)) {
        uniquify(box);
        box->at(p) = (box->at(p) & ~SLIMEBIT) | (slime << SLIMEBIT_SHIFT);
        return;
    }
    for (auto &d: other->data) {
        if (d.p == p) {
            uniquify(other);
            d.ch = (d.ch & ~SLIMEBIT) | (slime << SLIMEBIT_SHIFT);
            return;
        }
    }
    other->data.push_back({ p, OUT_CHAR | SLIMEBIT });
}

int Grid::bWidth() { return box->w; }
int Grid::bHeight() { return box->h; }