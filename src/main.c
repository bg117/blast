#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "ast_printer.h"
#include "parser.h"
#include "scanner.h"
#include "token.h"

char *get_multiline_input(void);

int main(int argc, char **argv)
{
    char *input = get_multiline_input();
    if (input == NULL) // no lines were read
        return 0;

    struct scanner scanner = { input };
    int            num_tokens;
    struct token  *tokens = scanner_scan(&scanner, &num_tokens);
    struct parser  parser = { tokens, num_tokens };
    struct ast    *ast    = parser_parse(&parser);

    ast_printer(ast);

    return 0;
}

char *get_multiline_input(void)
{
    char **lines     = malloc(sizeof(char *));
    int    num_lines = 0;

    printf(">>> ");

    while (1)
    {
        if (num_lines > 0)
            printf("... ");

        char  buf[1024];
        char *f = fgets(buf, 1024, stdin); // read a line from stdin

        if (f == NULL) // EOF
        {
            printf("\b\b\b\b\b\b      \n");
            break;
        }

        lines            = realloc(lines, sizeof(char *) * (num_lines + 1)); // allocate space for the new line
        lines[num_lines] = malloc(sizeof(char) * (strlen(buf) + 1));         // allocate space for the new line
        strcpy(lines[num_lines], buf);                                       // copy the line into the array
        num_lines++;
    }

    if (num_lines == 0)
        return NULL;

    int total_length = 1; // +1 for the null terminator
    for (int i = 0; i < num_lines; i++)
        total_length += strlen(lines[i]);

    char *result = malloc(sizeof(char) * total_length);
    result[0]    = '\0';
    for (int i = 0; i < num_lines; i++)
    {
        strcat(result, lines[i]);
        free(lines[i]);
    }

    free(lines);

    return result;
}
