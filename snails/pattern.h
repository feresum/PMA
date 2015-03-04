#include "state.h"

class Pattern {
public:
    virtual bool match(State s);
};
#include "general.h"

class P_Char : public Pattern { 
    virtual bool testch(int ch);
};

class P_CharAny : public P_Char { };

class P_CharExact : public P_Char { public: int c; };

class P_Sequence : public Pattern {};

class P_Alternation : public Pattern {};

class P_Assertion : public Pattern { public: bool value; };


class P_SetState : public Pattern {
    virtual void act();

    bool match() {
        act();
        return true; 
    }
};

class PS_Direction : public P_SetState { };

class PS_DirAbsolute : public PS_Direction { public: point dir; };

class PS_DirRelative : public PS_Direction { public: int angle; };