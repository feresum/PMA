enum m_start {
    M_START_TOPLEFT,
    M_START_ALLBOX
};

enum m_type {
    M_TYPE_COUNT,
    M_TYPE_BOOLEAN,
    M_TYPE_ALL
};

enum m_just {
    M_JUST_LEFT,
    M_JUST_RIGHT
};


#define MATCH_RESULT_OVERFLOW (-1)


int match(State initial, P_Sequence *p, bool allpaths = false);

