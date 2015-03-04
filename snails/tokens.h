#include "general.h"
#include "pattern.h"

class Token {};

class T_Pattern : public Token {
public:
    Pattern &p;
};

class T_Assert : public Token {
    bool value;
};

class T_DirList : public Token {
    std::vector<PS_Direction> list;
};

class T_GroupOpen : public Token {
    int type;
};

class T_GroupClose : public Token {
    int type;
};

class T_GroupDiv : public Token { };

class T_Quantifier : public Token { 
public:
    unsigned minimum, maximum; 
    T_Quantifier(unsigned mi, unsigned ma): minimum(mi), maximum(ma) { }
};