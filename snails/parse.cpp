#include "general.h"
#include "parse.h"
#include "pattern.h"
#include "instructions.h"
#include "tokens.h"

#ifdef DEBUG_PARSER
#include <iostream>
#endif

int read_ch(s_i &x) {
    return x.get();
}

bool digit(int ch) {
    return ch >= '0' && ch <= '9';
}

int read_num(s_i &x) {
    int num = 0;
    while (digit(x.peek())) {
        num = num * 10 + (x.get() - '0');
    }
    return num;
}

void skip_comment(s_i &x) {
    int next;
    while ((next = x.get()) != '\n' && ~next);
}

bool is_dir_inst(int ch, bool allowrel) {
    switch (ch) {
    case INST_D_RIGHT:
    case INST_D_UP:
    case INST_D_LEFT:
    case INST_D_DOWN:
    case INST_D_ORTHOGONAL:
    case INST_D_DIAGONAL:
    case INST_D_OCTILINEAR:
    case INST_D_STATIONARY:
    case INST_D_ABSNUM:
        return true;
    case INST_D_RELNUM:
    case INST_D_CLOCKWISE:
    case INST_D_COUNTERCLOCKWISE:
    case INST_D_FORWARD:
    case INST_D_BACKWARD:
    case INST_D_NORMAL:
        return allowrel;
    default:
        return false;
    }
}

vector<Direction> read_dirs(s_i &x, bool allowrel) {
    vector<Direction> v;
    do {
        char ch = x.get();
        switch (ch) {
        case INST_D_RIGHT:
            v.emplace_back( point{ 1, 0 } );
            break;
        case INST_D_UP:
            v.emplace_back( point{ 0, -1 } );
            break;
        case INST_D_LEFT:
            v.emplace_back( point{ -1, 0 } );
            break;
        case INST_D_DOWN:
            v.emplace_back( point{ 0, 1 } );
            break;
        case INST_D_ORTHOGONAL:
            v.emplace_back( point{ 1, 0 } );
            v.emplace_back( point{ 0, -1 } );
            v.emplace_back( point{ -1, 0 } );
            v.emplace_back( point{ 0, 1 } );
            break;
        case INST_D_DIAGONAL:
            v.emplace_back( point{ 1, -1 } );
            v.emplace_back( point{ -1, -1 } );
            v.emplace_back( point{ -1, 1 } );
            v.emplace_back( point{ 1, 1 } );
            break;
        case INST_D_OCTILINEAR:
            for (point p : DIRECTION_LIST) {
                v.emplace_back( p );
            }
            break;
        case INST_D_CLOCKWISE:
            v.emplace_back( 6 );
            break;
        case INST_D_COUNTERCLOCKWISE:
            v.emplace_back( 2 );
            break;
        case INST_D_FORWARD:
            v.emplace_back( 0 );
            break;
        case INST_D_BACKWARD:
            v.emplace_back( 4 );
            break;
        case INST_D_NORMAL:
            v.emplace_back( 2 );
            v.emplace_back( 6 );
            break;
        case INST_D_STATIONARY:
            v.emplace_back( point{ 0, 0 } );
            break;
        case INST_D_ABSNUM:
            v.emplace_back( DIRECTION_LIST[digit(x.peek()) ? x.get() & 7 : 1] );
            break;
        case INST_D_RELNUM:
            v.emplace_back( digit(x.peek()) ? x.get() & 7 : 1 );
            break;
        default:
            NEVERHAPPEN
        }
    } while (is_dir_inst(x.peek(), allowrel));
    return v;
}




static vector<Token*> tokenize(s_i &x) {
    vector<Token*> v;
    int ch;
    while (~(ch = x.get())) {
        if (is_dir_inst(ch, true)) {
            x.back();
            v.push_back(new T_Pattern{ new P_DirectionAlternation{ read_dirs(x, true) } });
        } else switch (ch) {
        case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
        {
            x.back();
            unsigned ll = read_num(x), ul;
            if (x.peek(0) == INST_QUANTIFIER_RANGE_OR_LINE_COMMENT && x.peek(1) != INST_QUANTIFIER_RANGE_OR_LINE_COMMENT) {
                x.get();
                ul = digit(x.peek()) ? read_num(x) : ~0U;
            } else {
                ul = ll;
            }
            v.push_back(new T_Quantifier{ ll, ul });
            break;
        }

        case INST_QUANTIFIER_RANGE_OR_LINE_COMMENT:
        {
            if (x.peek() == INST_QUANTIFIER_RANGE_OR_LINE_COMMENT) {
                skip_comment(x);
            } else {
                unsigned ul = digit(x.peek()) ? read_num(x) : ~0U;
                v.push_back(new T_Quantifier{ 0, ul });
            }
            break;
        }

        case INST_QUANTIFIER_AT_LEAST_1:
            if (x.peek() == INST_QUANTIFIER_RELUCTANT) {
                x.get();
                v.push_back(new T_Quantifier{ ~0U, 1 });
            } else {
                v.push_back(new T_Quantifier{ 1, ~0U });
            }
            break;

        case INST_QUANTIFIER_AT_MOST_1:
            if (x.peek() == INST_QUANTIFIER_RELUCTANT) {
                x.get();
                v.push_back(new T_Quantifier{ 1, 0 });
            } else {
                v.push_back(new T_Quantifier{ 0, 1 });
            }
            break;

        case INST_CHAR_LITERAL:
            v.push_back(new T_Pattern{ new P_CharExact{ x.get() } });
            break;

        case INST_CHAR_NEGATIVE_LITERAL:
            v.push_back(new T_Pattern{ new P_CharNegative{ x.get() } });
            break;

        case INST_CHAR_ANY:
            v.push_back(new T_Pattern{ new P_CharAny });
            break;

        case INST_CHAR_OUT:
            v.push_back(new T_Pattern{ new P_CharOut });
            break;

        case INST_ALTERNATION:
            v.push_back(new T_Alternator{});
            break;

        case INST_GROUP_OPEN1:
            v.push_back(new T_GroupOpen{ 1 });
            break;

        case INST_GROUP_OPEN2:
            v.push_back(new T_GroupOpen{ 2 });
            break;

        case INST_GROUP_CLOSE1:
            v.push_back(new T_GroupClose{ 1 });
            break;

        case INST_GROUP_CLOSE2:
            v.push_back(new T_GroupClose{ 2 });
            break;

        case INST_GROUP_DIV:
            v.push_back(new T_GroupDiv{});
            break;

        case INST_ASSERT_POSITIVE:
            v.push_back(new T_Assert{ true });
            break;
        case INST_ASSERT_NEGATIVE:
            v.push_back(new T_Assert{ false });
            break;

        case INST_TELEPORT:
            v.push_back(new T_Pattern{ new P_Teleport });
            break;

        case ' ':case '\t': case '\n': case '\r':
            break;

        default:
            throw parse_exc("Unrecognized instruction", x.ind());
        }//switch
    
    }
    return v;
}


static vector<Token*> insert_concatenators(const vector<Token*>& t) {
    vector<Token*> out;
    static T_Concatenation cat;
    static T_Pattern empty(new P_Sequence);
    bool firstInGroup = true;
    bool afterPrefix = false;
    out.push_back(new T_GroupOpen{ 0 });
    for (Token* tok : t) {
        if (tok->isUnaryPrefix() || tok->isAtom()) {
            if (!afterPrefix) {
                if (firstInGroup)
                    firstInGroup = false;
                else
                    out.push_back(&cat);
            }
        } else if (tok->isGroupOpener()) {
            if (!afterPrefix && !firstInGroup)
                out.push_back(&cat);
            firstInGroup = true;
        } else if (afterPrefix) {
            throw parse_exc("Nothing to assert");
        } else if (isA<T_Alternator>(tok)) {
            if (firstInGroup)
                out.push_back(&empty);
            firstInGroup = true;
        } else if (tok->isUnaryPostfix()) {
            if (firstInGroup)
                throw parse_exc("Nothing to quantify");
        } else {
            assert(tok->isGroupCloser());
            if (firstInGroup)
                out.push_back(&empty);
            firstInGroup = false;
        }
        out.push_back(tok);
        afterPrefix = tok->isUnaryPrefix();
    }
    if (afterPrefix)
        throw parse_exc("Nothing to assert");
    if (firstInGroup)
        out.push_back(&empty);
    out.push_back(new T_GroupClose{ 0 });
    return out;
}

P_Sequence* sequencify(Pattern* p) {
    P_Sequence* ps = dynamic_cast<P_Sequence*>(p);
    if (ps)
        return ps;
    ps = new P_Sequence;
    ps->v.push_back(p);
    return ps;
}

static bool should_eval(Token* next, vector<Token*>& opstk) {
    Token* top = opstk.back();
    if (next->precedence() <= top->precedence())
        return true;
    T_GroupClose* nextgc = dynamic_cast<T_GroupClose*>(next);
    if (!nextgc) return false;
    T_GroupOpen* topgo = dynamic_cast<T_GroupOpen*>(top);
    assert(topgo);
    if (topgo->type == nextgc->type) {
        opstk.pop_back();
        return false;
    }
    if (topgo->type == 0)
        return false;
    return true;
}
static P_Sequence* parse_tokenized(vector<Token*>& t) {
    vector<Token*> opstk;
    vector<Pattern*> atomstk;
    bool expectAtom = true; // as opposed to binary op
    for (Token* tok : t) {
        if (tok->isUnaryPostfix()) {
            //assume this has the highest precedence
            T_Quantifier* tq = (T_Quantifier*) tok;
            Pattern* p = pop(atomstk);
            P_Sequence* qseq = new P_Sequence;
            P_Quantifier* q = new P_Quantifier{ tq->minimum, tq->maximum };
            q->offset = 3;
            qseq->v.push_back(new P_QReset);
            qseq->v.push_back(q);
            qseq->v.push_back(p);
            qseq->v.push_back(new P_Jump{ -2 });
            P_Sequence* tseq = dynamic_cast<P_Sequence*>(p);
            if (tseq) {
                tseq->parent = qseq;
                tseq->iparent = 2;
            }
            atomstk.push_back(qseq);
        } else if (tok->isUnaryPrefix() || tok->isGroupOpener()) {
            assert(expectAtom);
            opstk.push_back(tok);
        } else if (tok->isAtom()) {
            assert(expectAtom);
            expectAtom = false;
            T_Pattern* tp = dynamic_cast<T_Pattern*>(tok);
            assert(tp);
            atomstk.push_back(tp->p);
        } else {
            assert(!expectAtom);
            while (should_eval(tok, opstk)) {
                Token* op = pop(opstk);
                if (op->isUnaryPrefix()) {
                    T_Assert* ta = dynamic_cast<T_Assert*>(op);
                    assert(ta);
                    P_Sequence* ps = sequencify(pop(atomstk));
                    ps->v.push_back(new P_Terminator);
                    atomstk.push_back(new P_Assertion{ ta->value, ps });
                } else if (op->isBinaryOp()) {
                    Pattern* right = pop(atomstk);
                    Pattern* left = pop(atomstk);
                    if (isA<T_Concatenation>(op)) {
                        P_Sequence* ps = sequencify(left);
                        P_Sequence* rps = dynamic_cast<P_Sequence*>(right);
                        if (rps) {
                            rps->parent = ps;
                            rps->iparent = ps->v.size();
                        }
                        ps->v.push_back(right);
                        atomstk.push_back(ps);
                    } else {
                        assert(isA<T_Alternator>(op));
                        P_Alternation *alt = new P_Alternation;
                        alt->v.push_back(sequencify(left));
                        alt->v.push_back(sequencify(right));
                        atomstk.push_back(alt);
                    }
                }
            }
            if (tok->isBinaryOp()) {
                expectAtom = true;
                opstk.push_back(tok);
            }
        }
    }
    assert(atomstk.size() == 1);
    assert(opstk.size() == 0);
    return sequencify(pop(atomstk));
}


P_Sequence * parse(s_i &prog) {
    auto tokens0 = tokenize(prog);
    auto tokens1 = insert_concatenators(tokens0);
#ifdef DEBUG_PARSER
    std::cerr << "Preliminary tokens:";
    for (Token* t : tokens0) std::cerr << ' ' << (str)*t;
    std::cerr << "\nModified tokens:";
    for (Token* t : tokens1) std::cerr << ' ' << (str)*t;
    std::cerr << '\n';
#endif
    P_Sequence *ps = parse_tokenized(tokens1);
    ps->v.push_back(new P_Terminator);
    return ps;
}

