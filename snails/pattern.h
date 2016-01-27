#ifndef H_PATTERN
#define H_PATTERN

#include <type_traits>

#include "state.h"
#include "general.h"

class StateP;


class Pattern {
};
int Pattern_match(Pattern* p, vector<StateP>& stk);

#define PTYPE_LIST_APPLY(MACRO) \
    MACRO(P_CharAny) \
    MACRO(P_CharOut) \
    MACRO(P_CharExact) \
    MACRO(P_CharNegative) \
    MACRO(P_Sequence) \
    MACRO(P_Alternation) \
    MACRO(P_Assertion) \
    MACRO(P_DirectionAlternation) \
    MACRO(P_Teleport) \
    MACRO(P_Terminator) \
    MACRO(P_Jump) \
    MACRO(P_Quantifier) \
    MACRO(P_QReset) \

#define APPEND_COMMA(W) W,
enum class PType
{
    PTYPE_LIST_APPLY(APPEND_COMMA)
};

template<typename PT> Pattern* rcps(PT* p) {
    static_assert(std::is_same<const PType, decltype(PT::static_type)>::value, "wrong");
    return reinterpret_cast<Pattern*>(p);
}
template<typename PT_ptr> PT_ptr PT_dynamic_cast(Pattern* p) {
    typedef typename std::remove_pointer<PT_ptr>::type PT;
    if (PT::static_type == *reinterpret_cast<PType*>(p))
        return reinterpret_cast<PT_ptr>(p);
    else
        return nullptr;
}

#define PTYPE(NAME) \
    PType type = PType::NAME; \
    static const PType static_type = PType::NAME;



struct P_CharAny {
    PTYPE(P_CharAny)

    int match(vector<StateP> &stk);
    bool testch(int ch) const { return ch >= 0; }
};

struct P_CharOut {
    PTYPE(P_CharOut)

    int match(vector<StateP> &stk);
    bool testch(int ch) const { return ch < 0; }
};

struct P_CharExact {
    PTYPE(P_CharExact)

    int c;
    P_CharExact(int c) : c(c) { }

    int match(vector<StateP> &stk);
    bool testch(int ch) const { 
        return c == ch; 
    }
};

struct P_CharNegative {
    PTYPE(P_CharNegative)

    int c;
    P_CharNegative(int c) : c(c) { }

    int match(vector<StateP> &stk);
    bool testch(int ch) const {
        return ch >= 0 && c != ch;
    }
};

struct P_Sequence {
    PTYPE(P_Sequence)

    vector<Pattern*> v;
    P_Sequence *parent;
    int iparent;

    int match(vector<StateP> &stk);
};

struct P_Alternation {
    PTYPE(P_Alternation)

    vector<P_Sequence*> v;
    int match(vector<StateP> &stk);
};

struct P_Assertion {
    PTYPE(P_Assertion)
 
    bool value; 
    P_Sequence *pattern;
    
    P_Assertion(bool value, P_Sequence *pattern) : value(value), pattern(pattern) { }

    int match(vector<StateP> &stk);
};




struct Direction {

    enum DType { ABSOLUTE, RELATIVE } type;
    int x[2];

    Direction(point dx): type(ABSOLUTE) {
        x[0] = dx.x;
        x[1] = dx.y;
    }
    Direction(int rdir): type(RELATIVE) {
        x[0] = rdir & 7;
    }
    Direction(const Direction &o) = default;

    point getDir(point prevdir); 

    point absdir() { return { x[0], x[1] }; }
};

struct P_DirectionAlternation {
    PTYPE(P_DirectionAlternation)

    vector<Direction> list;
    P_DirectionAlternation(vector<Direction> list) : list(list) { }

    int match(vector<StateP> &stk);
};



struct P_Teleport {
    PTYPE(P_Teleport)
    int match(vector<StateP> &stk);
};

struct P_Terminator {
    PTYPE(P_Terminator)
    int match(vector<StateP> &stk);
};

struct P_Jump {
    PTYPE(P_Jump)

    int offset;
    P_Jump(int offset) : offset(offset) { }
    int match(vector<StateP> &stk);
};

struct P_Quantifier {
    PTYPE(P_Quantifier)

    unsigned minimum, maximum;
    bool reluctant;
    int offset;

    int match(vector<StateP> &stk);
};

struct P_QReset {
    PTYPE(P_QReset)

    int match(vector<StateP> &stk);
};


#endif
