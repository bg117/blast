#ifndef TOKEN_H
#define TOKEN_H

/**
 * @brief Enum representing the different types of tokens.
 */
enum token_type
{
    END_OF_FILE,

    NUMBER,     /* integer */
    STRING,     /* string */
    IDENTIFIER, /* variable name */

    PLUS,  /* plus (+) */
    MINUS, /* minus (-) */
    MUL,   /* multiplication (*) */
    DIV,   /* division (/) */
    MOD,   /* modulo (%) */
    EXP,   /* exponentiation (**) */

    COLON, /* colon (:) for assignment */

    EQ, /* equal to (=) */
    NE, /* not equal to (<>) */
    LT, /* less than (<) */
    LE, /* less than or equal to (<=) */
    GT, /* greater than (>) */
    GE, /* greater than or equal to (>=) */

    PERIOD, /* period (.) for ending a statement */

    LPAREN, /* left parenthesis ( */
    RPAREN, /* right parenthesis ) */

    IF,   /* if keyword */
    THEN, /* then keyword */
    ELSE, /* else keyword */
    END,  /* end keyword */

    WHILE, /* while keyword */
    DO,    /* do keyword */

    ROUTINE, /* routine keyword (for defining a function) */
};

/**
 * @brief Struct representing a token.
 *
 * A token is a lexical unit of a programming language. It is a sequence of characters
 * that have a meaning in the programming language. For example, the token '1' is a
 * NUMBER token, and the token '+' is a PLUS token.
 */
struct token
{
    int   type;
    char *lexeme;
};

#endif
