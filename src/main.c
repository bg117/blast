#include <stdio.h>

#include "ast.h"
#include "ast_printer.h"
#include "parser.h"
#include "scanner.h"
#include "token.h"

int main(int argc, char **argv)
{
    char          *src     = "1 + 2 * 3";
    struct scanner scanner = { src };

    int           num_tokens;
    struct token *tokens = scanner_scan(&scanner, &num_tokens);

    for (int i = 0; i < num_tokens; i++)
        printf("%d: %s\n", tokens[i].type, tokens[i].lexeme);

    struct parser parser = { tokens, num_tokens };
    struct ast   *ast    = parser_parse(&parser);

    ast_printer(ast);

    return 0;
}
