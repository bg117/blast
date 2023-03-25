#include "parser.h"

#include <stdbool.h>
#include <stdio.h>
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

struct ast *expr_number(struct parser *parser)
{
    struct token *token    = consume(parser, (int[]){ TOKEN_NUMBER }, 1); // consume number token
    struct ast   *ast      = malloc(sizeof(struct ast));                  // allocate memory for ast
    ast->type              = AST_EXPR_NUMBER;                             // number ast
    ast->expr.number.value = atof(token->lexeme);                         // set ast value
    return ast;
}

struct ast *expr_string(struct parser *parser)
{
    struct token *token    = consume(parser, (int[]){ TOKEN_STRING }, 1); // consume string token
    struct ast   *ast      = malloc(sizeof(struct ast));                  // allocate memory for ast
    ast->type              = AST_EXPR_STRING;                             // string ast
    ast->expr.string.value = token->lexeme;                               // set ast value
    return ast;
}

struct ast *expr_variable(struct parser *parser)
{
    struct token *token     = consume(parser, (int[]){ TOKEN_IDENTIFIER }, 1); // consume variable token
    struct ast   *ast       = malloc(sizeof(struct ast));                      // allocate memory for ast
    ast->type               = AST_EXPR_VARIABLE;                               // variable ast
    ast->expr.variable.name = token->lexeme;                                   // set ast name
    return ast;
}

struct ast *expr_primary(struct parser *parser)
{
    if (check(parser, (int[]){ TOKEN_NUMBER }, 1)) // if current token is number
        return expr_number(parser);                // parse number expression

    if (check(parser, (int[]){ TOKEN_STRING }, 1)) // if current token is string
        return expr_string(parser);                // parse string expression

    if (check(parser, (int[]){ TOKEN_IDENTIFIER }, 1)) // if current token is variable
        return expr_variable(parser);                  // parse variable expression

    fprintf(stderr, "error: expected expression\n");
    return NULL;
}

struct ast *expr_unary(struct parser *parser)
{
    if (check(parser, (int[]){ TOKEN_MINUS }, 1)) // if current token is minus
    {
        struct token *token  = consume(parser, (int[]){ TOKEN_MINUS }, 1); // consume minus token
        struct ast   *ast    = malloc(sizeof(struct ast));                 // allocate memory for ast
        ast->type            = AST_EXPR_UNARY;                             // unary ast
        ast->expr.unary.op   = token->type;                                // set ast operator
        ast->expr.unary.expr = expr_unary(parser);                         // set ast right hand side
        return ast;
    }

    return expr_primary(parser);
}

struct ast *expr_exponent(struct parser *parser)
{
    struct ast *ast = expr_unary(parser); // parse unary expression

    if (check(parser, (int[]){ TOKEN_EXP }, 1)) // while current token is exponent
    {
        struct token *token    = consume(parser, (int[]){ TOKEN_EXP }, 1); // consume exponent token
        struct ast   *rhs      = expr_exponent(parser);                    // right-recurse exponent expression
        struct ast   *lhs      = ast;                                      // save initial ast
        ast                    = malloc(sizeof(struct ast));               // allocate memory for new ast
        ast->type              = AST_EXPR_BINARY;                          // binary ast
        ast->expr.binary.op    = token->type;                              // set ast operator
        ast->expr.binary.left  = lhs;                                      // set ast left hand side
        ast->expr.binary.right = rhs;                                      // set ast right hand side
    }

    return ast;
}

struct ast *expr_multiplicative(struct parser *parser)
{
    struct ast *ast     = expr_exponent(parser); // parse exponent expression
    int         types[] = { TOKEN_MUL, TOKEN_DIV };

    while (check(parser, types, 2)) // while current token is star or slash
    {
        struct token *token    = consume(parser, types, 2);  // consume star or slash token
        struct ast   *rhs      = expr_exponent(parser);      // parse inner exponent expression
        struct ast   *lhs      = ast;                        // save initial ast
        ast                    = malloc(sizeof(struct ast)); // allocate memory for new ast
        ast->type              = AST_EXPR_BINARY;            // binary ast
        ast->expr.binary.op    = token->type;                // set ast operator
        ast->expr.binary.left  = lhs;                        // set ast left hand side
        ast->expr.binary.right = rhs;                        // set ast right hand side
    }

    return ast;
}

struct ast *parser_parse(void)
{
    struct ast *root = malloc(sizeof(struct ast));
    return root;
}
