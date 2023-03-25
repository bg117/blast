#include "parser.h"

#include <stdbool.h>
#include <stdlib.h>

#include "ast.h"
#include "token.h"

#define SIZE(array) (sizeof(array) / sizeof((array)[0]))

bool is_at_end(struct parser *parser)
{
    return parser->i >= parser->num_tokens; // current token is last token
}

struct token *peek(struct parser *parser)
{
    return &parser->tokens[parser->i]; // reference to current token
}

bool check(struct parser *parser, int *types, int num_types)
{
    if (is_at_end(parser))
        return false;

    for (int i = 0; i < num_types; i++)     // for each type
        if (peek(parser)->type == types[i]) // if current token is one of the types
            return true;                    // return true

    return false; // current token is not one of the types
}

void advance(struct parser *parser)
{
    if (!is_at_end(parser))
        parser->i++;
}

struct token *consume(struct parser *parser, int *types, int num_types)
{
    if (check(parser, types, num_types)) // if current token is one of the types
    {
        struct token *token = peek(parser); // reference to current token
        advance(parser);                    // advance to next token
        return token;                       // return reference to current token
    }

    return NULL;
}

struct ast *parser_parse(void)
{
    struct ast *root = malloc(sizeof(struct ast));
    return root;
}
