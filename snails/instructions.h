#ifndef H_INSTRUCTIONS
#define H_INSTRUCTIONS

#define INST_OPTION_RJUST 'R'
#define INST_OPTION_TYPE_BOOLEAN '?'
#define INST_OPTION_TYPE_ALL '&'
#define INST_OPTION_START_TOPLEFT '^'
#define INST_OPTION_FILL_SPACE 'F'
#define INST_OPTION_FILL_CHAR 'E'
#define INST_OPTION_MOBILE_START 'M'
#define INST_OPTION_ALL_PATHS 'A'

#define INST_D_RIGHT 'r'
#define INST_D_UP 'u'
#define INST_D_LEFT 'l'
#define INST_D_DOWN 'd'
#define INST_D_ORTHOGONAL 'o'
#define INST_D_DIAGONAL 'y'
#define INST_D_OCTILINEAR 'z'
#define INST_D_CLOCKWISE 'c'
#define INST_D_COUNTERCLOCKWISE 'w'
#define INST_D_FORWARD 'f'
#define INST_D_BACKWARD 'b'
#define INST_D_NORMAL 'n'
#define INST_D_STATIONARY 's'
#define INST_D_ABSNUM 'a'
#define INST_D_RELNUM 'e'

#define INST_TELEPORT 't'

#define INST_GROUP_OPEN1 '('
#define INST_GROUP_OPEN2 '{'
#define INST_GROUP_CLOSE1 ')'
#define INST_GROUP_CLOSE2 '}'
#define INST_GROUP_DIV '`'

#define INST_ALTERNATION '|'

#define INST_CHAR_LITERAL '\\'
#define INST_CHAR_NEGATIVE_LITERAL '^'
#define INST_CHAR_ANY '.'
#define INST_CHAR_OUT '~'

#define INST_ESCAPE '\\'

#define INST_QUANTIFIER_RANGE ','
#define INST_QUANTIFIER_AT_MOST_1 '?'
#define INST_QUANTIFIER_AT_LEAST_1 '+'
#define INST_QUANTIFIER_RELUCTANT '?'

#define INST_ASSERT_POSITIVE '='
#define INST_ASSERT_NEGATIVE '!'

#endif
