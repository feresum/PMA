#ifndef H_TOKENS
#define H_TOKENS

#include "general.h"
#include "pattern.h"

class Token {
public:
    virtual ~Token(); 
};
Token::~Token() { }

class T_Pattern : public Token {
public:
    Pattern *p;
    T_Pattern(Pattern *p) : p(p) { }
};

class T_Assert : public Token {
    bool value;
};


class T_GroupOpen : public Token {
public:
    int type;
    T_GroupOpen(int type) : type(type) { }
};

class T_GroupClose : public Token {
public:
    int type;
    T_GroupClose(int type) : type(type) { }
};

class T_GroupDiv : public Token {};

class T_Quantifier : public Token { 
public:
    unsigned minimum, maximum; 
    T_Quantifier(unsigned mi, unsigned ma): minimum(mi), maximum(ma) { }
};

class T_Alternator : public Token {};

#endif
