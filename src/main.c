#include "scanner.h"
#include "token.h"

#include <stdio.h>

int main(int argc, char **argv)
{
    char          *src     = "1 + 2 * 3";
    struct scanner scanner = { .src = src };

    int           num_tokens;
    struct token *tokens = scanner_scan(&scanner, &num_tokens);

    for (int i = 0; i < num_tokens; i++)
        printf("%d: %s\n", tokens[i].type, tokens[i].lexeme);
}
