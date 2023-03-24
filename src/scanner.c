#include "scanner.h"

#include <ctype.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
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

void scan_single(struct scanner *scanner, struct token **tokens, int *num_tokens)
{
    // skip whitespace
    skip_whitespace(scanner);

    // if at end of source, return
    if (is_at_end(scanner))
        return;

    // for each pattern
    for (int i = 0; i < sizeof(PATTERNS) / sizeof(PATTERNS[0]); i++)
    {
        regex_t    regex;
        regmatch_t match;
        regcomp(&regex, PATTERNS[i].value, REG_EXTENDED); // compile regex

        // substring of source from current position
        char *substring = scanner->src + scanner->i;

        // if not match, continue
        if (regexec(&regex, substring, 1, &match, 0) != 0)
            continue;

        // if match, add token to token s
        char *lexeme = malloc(match.rm_eo - match.rm_so + 1); // allocate memory for lexeme (plus null terminator)
        strncpy(lexeme, substring + match.rm_so, match.rm_eo - match.rm_so); // copy lexeme from source
        lexeme[match.rm_eo - match.rm_so] = '\0';                            // null terminate lexeme

        struct token token = {
            .type   = PATTERNS[i].key,
            .lexeme = lexeme,
        };

        *tokens = realloc(*tokens, sizeof(struct token) * (*num_tokens + 1)); // allocate memory for new token
        (*tokens)[*num_tokens] = token;                                       // add token to tokens
        (*num_tokens)++;                                                      // increment number of tokens
    }

    // if no pattern matches, error
    fprintf(stderr, "error: unexpected character '%c'", scanner->src[scanner->i]);
    if (!is_at_end(scanner))
        scanner->i++; // skip current character
}

struct token *scanner_scan(struct scanner *scanner, int *num_tokens)
{
    *num_tokens          = 0;    // no tokens yet
    struct token *tokens = NULL; // scan_single will call realloc to allocate memory for tokens

    // while not at end of source
    while (!is_at_end(scanner))
        scan_single(scanner, &tokens, num_tokens); // scan single token

    return tokens;
}
