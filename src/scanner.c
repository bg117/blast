#include "scanner.h"

#include "key_value_pair.h"
#include "token.h"

struct key_value_pair patterns[] = {
    { NUMBER, "(\\d+(?:\\.\\d+)?)" },
    { STRING, "\"([^\"]*)\"" },
    { PLUS, "(\\+)" },
    { MINUS, "(-)" },
    /* must be before MUL to avoid matching ** */
    { EXP, "(\\*\\*)" },
    { MUL, "(\\*)" },
    { DIV, "(/)" },
    { MOD, "(%)" },
    { COLON, "(:)" },
    { NE, "(<>)" },
    { LE, "(<=)" },
    { GE, "(>=)" },
    { LT, "(<)" },
    { GT, "(>)" },
    /* must be before LE and GE to avoid matching <= and >= */
    { EQ, "(=)" },
    { PERIOD, "(\\.)" },
    { LPAREN, "(\\()" },
    { RPAREN, "(\\))" },
    { IF, "(if)" },
    { THEN, "(then)" },
    { ELSE, "(else)" },
    { END, "(end)" },
    { WHILE, "(while)" },
    { DO, "(do)" },
    { ROUTINE, "(routine)" },
    /* must be last to avoid matching keywords */
    { IDENTIFIER, "(\\w+)" },
};
