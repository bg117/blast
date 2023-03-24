#include "scanner.h"

#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#include "key_value_pair.h"
#include "token.h"

const struct key_value_pair PATTERNS[] = {
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

bool is_at_end(struct scanner *scanner)
{
    return scanner->i >= strlen(scanner->src);
}

void skip_whitespace(struct scanner *scanner)
{
    // while not at end of source and current character is whitespace
    while (!is_at_end(scanner) && isspace(scanner->src[scanner->i]))
        scanner->i++; // skip current character
}

void scanner_scan(struct scanner *scanner, struct token *tokens)
{
}
