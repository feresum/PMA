#include "ast.h"
#include "pattern.h"
#include "compile.h"


static P_Sequence* sequencify(Pattern* p) {
    P_Sequence* ps = dynamic_cast<P_Sequence*>(p);
    if (ps)
        return ps;
    ps = new P_Sequence;
    ps->v.push_back(p);
    return ps;
}

static Pattern* compile(AST::Pattern* p);


static Pattern* compile(AST::EmptyPattern* p) {
    return new P_Sequence;
}

static Pattern* compile(AST::Concatenation* p) {
    P_Sequence* ps = sequencify(compile(p->child[0]));
    Pattern* right = compile(p->child[1]);
    P_Sequence* rps = dynamic_cast<P_Sequence*>(right);
    if (rps) {
        rps->parent = ps;
        rps->iparent = ps->v.size();
    }
    ps->v.push_back(right);
    return ps;
}

static Pattern* compile(AST::Alternation* p) {
    P_Alternation *alt = new P_Alternation;
    alt->v.push_back(sequencify(compile(p->child[0])));
    alt->v.push_back(sequencify(compile(p->child[1])));
    return alt;
}

static Pattern* compile(AST::Assertion* p) {
    return new P_Assertion(p->value, compile_top(p->child));
}

static Pattern* compile(AST::Quantifier* q) {
    auto* pq = new P_Quantifier;
    auto* qseq = new P_Sequence;
    Pattern* ch = compile(q->child);

    pq->minimum = q->minimum;
    pq->maximum = q->maximum;
    pq->reluctant = q->reluctant;
    pq->offset = 3;

    qseq->v.push_back(new P_QReset);
    qseq->v.push_back(pq);
    qseq->v.push_back(ch);
    qseq->v.push_back(new P_Jump{ -2 });

    P_Sequence* tseq = dynamic_cast<P_Sequence*>(ch);
    if (tseq) {
        tseq->parent = qseq;
        tseq->iparent = 2;
    }
    return qseq;
}

static Pattern* compile(AST::MatchCharSingle* p) {
    return new P_CharExact{ p->ch };
}
static Pattern* compile(AST::MatchCharNegative* p) {
    return new P_CharNegative{ p->ch };
}
static Pattern* compile(AST::MatchCharAny*) {
    return new P_CharAny;
}
static Pattern* compile(AST::MatchCharOut*) {
    return new P_CharOut;
}
static Pattern* compile(AST::Teleport* p) {
    return new P_Teleport;
}
static Pattern* compile(AST::DirectionAlternation* p) {
    return new P_DirectionAlternation{ p->dirs };
}

template<class PClass> struct compile_template {
    Pattern* operator()(AST::Pattern* p) {
        return compile((PClass*)p);
    }
};
static Pattern* compile(AST::Pattern* p) {
    return AST::dispatch<compile_template>(p);
}

P_Sequence* compile_top(AST::Pattern* p) {
    P_Sequence* res = sequencify(compile(p));
    res->v.push_back(new P_Terminator);
    return res;
}
