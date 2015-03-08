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

enum match_result {
    MATCH_RESULT_FAIL,
    MATCH_RESULT_SUCCESS,
    MATCH_RESULT_OVERFLOW
};

match_result match(State initial, P_Sequence *p);

