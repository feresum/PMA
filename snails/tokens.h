#ifndef H_TOKENS
#define H_TOKENS

#include "general.h"
#include "pattern.h"
#include "ast.h"

class Token {
public:
    virtual ~Token(); 

    virtual bool isAtom() { return false; }
    virtual bool isBinaryOp() { return false; }
    virtual bool isUnaryPrefix() { return false; }
    virtual bool isUnaryPostfix() { return false; }
    virtual bool isGroupOpener() { return false; }
    virtual bool isGroupCloser() { return false; }

	typedef int precedence_t;
    virtual precedence_t precedence() { NEVERHAPPEN }

#ifdef DEBUG_PARSER
#ifdef _MSC_VER
#define explicit
#endif
    virtual explicit operator str() = 0;
#endif

};
Token::~Token() { }

class T_Pattern : public Token {
public:
    AST::Pattern *p;
    T_Pattern(AST::Pattern *p) : p(p) { }
    bool isAtom() { return true; }
#ifdef DEBUG_PARSER
    explicit operator str() {
        return "P";
    }
#endif
};

class T_Assert : public Token {
public:
    bool value;
    AST::Pattern *teleport = nullptr, *direction = nullptr;
    T_Assert(bool value) : value(value) { }
    bool isUnaryPrefix() { return true; }
    precedence_t precedence() { return 30; }
#ifdef DEBUG_PARSER
    explicit operator str() {
        str s = value ? "=" : "!";
        if (teleport) { s += "(+t)"; }
        if (direction) { s += "(+dir)"; }
        return s;
    }
#endif
};


class T_GroupOpen : public Token {
public:
    int type;
    T_GroupOpen(int type) : type(type) { }
    bool isGroupOpener() { return true; }
    precedence_t precedence() { return -20; }
#ifdef DEBUG_PARSER
    explicit operator str() {
        return str("open(") + std::to_string(type) + ')';
    }
#endif
};

class T_GroupClose : public Token {
public:
    int type;
    T_GroupClose(int type) : type(type) { }
    bool isGroupCloser() { return true; }
    precedence_t precedence() { return -10; }
#ifdef DEBUG_PARSER
    explicit operator str() {
        return str("close(") + std::to_string(type) + ')';
    }
#endif
};

class T_GroupDiv : public Token {

    bool isGroupCloser() { return true; }
    precedence_t precedence() { return 10; }
#ifdef DEBUG_PARSER
    explicit operator str() {
        return "`";
    }
#endif
};

class T_Quantifier : public Token { 
public:
    unsigned minimum, maximum; 
    T_Quantifier(unsigned mi, unsigned ma): 
        minimum(mi), maximum(ma) {}
    bool isUnaryPostfix() { return true; }
    precedence_t precedence() { return 40; }
#ifdef DEBUG_PARSER
    explicit operator str() {
        return str("Q(") + std::to_string(minimum) + ',' + std::to_string(maximum) + ')';
    }
#endif
};

class T_Alternator : public Token {
    bool isBinaryOp() { return true; }
    precedence_t precedence() { return 0; }
#ifdef DEBUG_PARSER
    explicit operator str() {
        return "|";
    }
#endif
};

class T_Concatenation : public Token {
    bool isBinaryOp() { return true; }
    precedence_t precedence() { return 20; }
#ifdef DEBUG_PARSER
    explicit operator str() {
        return "++";
    }
#endif
};


#endif
