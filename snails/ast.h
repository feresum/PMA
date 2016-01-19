#ifndef H_AST
#define H_AST

#include "pattern.h"

namespace AST
{
    // ---- abstract Patterns ----

    struct Pattern {
        virtual ~Pattern() {}
    };

    struct Leaf : Pattern {
        virtual ~Leaf() {}
    };

    struct Unary : Pattern {
        Pattern* child;
        virtual ~Unary() {}
    };

    struct Binary : Pattern {
        Pattern* child[2];
        virtual ~Binary() {}
    };

    // ---- instantiated Patterns ----

    struct EmptyPattern : Leaf {};

    struct PrecompiledPattern : Leaf {
        ::Pattern* pat;
    };

    struct Alternation : Binary {};

    struct Concatenation : Binary {};

    struct Assertion : Unary {
        bool value;
    };

    struct Quantifier : Unary {
        unsigned minimum, maximum;
        bool reluctant;
        Quantifier(unsigned a, unsigned b) {
            minimum = std::min(a, b);
            maximum = std::max(a, b);
            reluctant = b < a;
        }
    };


    template<template<class> typename Func, typename... T> auto dispatch(Pattern* p, T&&... args) -> decltype(Func<Pattern>()(p, args...)) {
        if (isA<EmptyPattern>(p)) return Func<EmptyPattern>()(p, args...);
        if (isA<PrecompiledPattern>(p)) return Func<PrecompiledPattern>()(p, args...);
        if (isA<Alternation>(p)) return Func<Alternation>()(p, args...);
        if (isA<Concatenation>(p)) return Func<Concatenation>()(p, args...);
        if (isA<Assertion>(p)) return Func<Assertion>()(p, args...);
        if (isA<Quantifier>(p)) return Func<Quantifier>()(p, args...);
        NEVERHAPPEN
    }

}

#endif
