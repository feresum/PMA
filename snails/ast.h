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

    struct MatchChar : Leaf {
        virtual ~MatchChar() {}
    };

    // ---- instantiated Patterns ----

    struct EmptyPattern : Leaf {};

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

    struct MatchCharSingle : MatchChar {
        int ch;
        MatchCharSingle(int ch) : ch(ch) {}
    };

    struct MatchCharNegative : MatchChar {
        int ch;
        MatchCharNegative(int ch) : ch(ch) {}
    };

    struct MatchCharAny : MatchChar {};

    struct MatchCharOut : MatchChar {};

    struct Teleport : Leaf {};

    struct DirectionAlternation : Leaf {
        vector<Direction> dirs;
        DirectionAlternation(const vector<Direction>& dirs) : dirs(dirs) {}
    };


    template<template<class> typename Func, typename... T> auto dispatch(Pattern* p, T&&... args) -> decltype(Func<Pattern>()(p, args...)) {
#define DISPATCH_CLASS(CLASS) if (isA<CLASS>(p)) return Func<CLASS>()(p, args...)

        DISPATCH_CLASS(EmptyPattern);
        DISPATCH_CLASS(Alternation);
        DISPATCH_CLASS(Concatenation);
        DISPATCH_CLASS(Assertion);
        DISPATCH_CLASS(Quantifier);
        DISPATCH_CLASS(MatchCharSingle);
        DISPATCH_CLASS(MatchCharNegative);
        DISPATCH_CLASS(MatchCharAny);
        DISPATCH_CLASS(MatchCharOut);
        DISPATCH_CLASS(Teleport);
        DISPATCH_CLASS(DirectionAlternation);
        NEVERHAPPEN
    }

}

#endif
