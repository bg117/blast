#ifndef TOKEN_H
#define TOKEN_H

/**
 * @brief Enum representing the different types of tokens.
 */
enum token_type
{
    TOKEN_EOF,

    TOKEN_NUMBER,     /* integer */
    TOKEN_STRING,     /* string */
    TOKEN_IDENTIFIER, /* variable name */

    TOKEN_PLUS,  /* plus (+) */
    TOKEN_MINUS, /* minus (-) */
    TOKEN_MUL,   /* multiplication (*) */
    TOKEN_DIV,   /* division (/) */
    TOKEN_MOD,   /* modulo (%) */
    TOKEN_EXP,   /* exponentiation (**) */

    TOKEN_COLON, /* colon (:) for assignment */

    TOKEN_EQ, /* equal to (=) */
    TOKEN_NE, /* not equal to (<>) */
    TOKEN_LT, /* less than (<) */
    TOKEN_LE, /* less than or equal to (<=) */
    TOKEN_GT, /* greater than (>) */
    TOKEN_GE, /* greater than or equal to (>=) */

    TOKEN_PERIOD, /* period (.) for ending a statement */

    TOKEN_LPAREN, /* left parenthesis ( */
    TOKEN_RPAREN, /* right parenthesis ) */

    TOKEN_IF,   /* if keyword */
    TOKEN_THEN, /* then keyword */
    TOKEN_ELSE, /* else keyword */
    TOKEN_END,  /* end keyword */

    TOKEN_WHILE, /* while keyword */
    TOKEN_DO,    /* do keyword */

    TOKEN_ROUTINE, /* routine keyword (for defining a function) */
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

/**
 * @brief Converts a token type to a string.
 *
 * @param type The token type.
 * @return const char* The string representation of the token type.
 */
const char *token_type_to_string(int type);

#endif
