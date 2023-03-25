#include "scanner.h"

#include <ctype.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "key_value_pair.h"
#include "token.h"

static const struct key_value_pair PATTERNS[] = {
    { TOKEN_NUMBER, "^[0-9]+(\\.[0-9]+)?" },
    { TOKEN_STRING, "^\"[^\"]*\"" },
    { TOKEN_PLUS, "^\\+" },
    { TOKEN_MINUS, "^-" },
    /* must be before MUL to avoid matching ** */
    { TOKEN_EXP, "^\\*\\*" },
    { TOKEN_MUL, "^\\*" },
    { TOKEN_DIV, "^/" },
    { TOKEN_MOD, "^%" },
    { TOKEN_COLON, "^:" },
    { TOKEN_NE, "^<>" },
    { TOKEN_LE, "^<=" },
    { TOKEN_GE, "^>=" },
    { TOKEN_LT, "^<" },
    { TOKEN_GT, "^>" },
    /* must be before LE and GE to avoid matching <= and >= */
    { TOKEN_EQ, "^=" },
    { TOKEN_PERIOD, "^\\." },
    { TOKEN_LPAREN, "^\\(" },
    { TOKEN_RPAREN, "^\\)" },
    { TOKEN_IF, "^if" },
    { TOKEN_THEN, "^then" },
    { TOKEN_ELSE, "^else" },
    { TOKEN_END, "^end" },
    { TOKEN_WHILE, "^while" },
    { TOKEN_DO, "^do" },
    { TOKEN_ROUTINE, "^routine" },
    /* must be last to avoid matching keywords */
    { TOKEN_IDENTIFIER, "^[A-Za-z_$][A-Za-z0-9_$]*" },
};

static bool is_at_end(struct scanner *scanner)
{
    return scanner->i >= strlen(scanner->src);
}

static void skip_whitespace(struct scanner *scanner)
{
    // while not at end of source and current character is whitespace
    while (!is_at_end(scanner) && isspace(scanner->src[scanner->i]))
        scanner->i++; // skip current character
}

static void scan_single(struct scanner *scanner, struct token **tokens, int *num_tokens)
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

        // if match, add token to tokens
        int start = match.rm_so;
        int end   = match.rm_eo;
        if (PATTERNS[i].key == TOKEN_STRING)
        {
            start++; // skip opening quote
            end--;   // skip closing quote
        }

        char *lexeme = malloc(end - start + 1); // allocate memory for lexeme (plus null terminator)

        strncpy(lexeme, substring + start, end - start); // copy lexeme from source
        lexeme[end - start] = '\0';                      // null terminate lexeme

        struct token token = {
            .type   = PATTERNS[i].key,
            .lexeme = lexeme,
        };

        *tokens = realloc(*tokens, sizeof(struct token) * (*num_tokens + 1)); // allocate memory for new token
        (*tokens)[*num_tokens] = token;                                       // add token to tokens
        (*num_tokens)++;                                                      // increment number of tokens

        scanner->i += match.rm_eo; // skip matched characters

        return;
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
