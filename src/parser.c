#include "parser.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "token.h"

static bool          is_at_end(struct parser *parser);
static struct token *peek(struct parser *parser);
static bool          check(struct parser *parser, int *types, int num_types);
static void          advance(struct parser *parser);
static struct token *consume(struct parser *parser, int *types, int num_types);

static struct ast *expr_number(struct parser *parser);
static struct ast *expr_string(struct parser *parser);
static struct ast *expr_variable(struct parser *parser);
static struct ast *expr_primary(struct parser *parser);
static struct ast *expr_unary(struct parser *parser);
static struct ast *expr_exponent(struct parser *parser);
static struct ast *expr_multiplicative(struct parser *parser);
static struct ast *expr_additive(struct parser *parser);
static struct ast *expr_relational(struct parser *parser);
static struct ast *expr_equality(struct parser *parser);
static struct ast *expr_assignment(struct parser *parser);
static struct ast *expr(struct parser *parser);

static struct ast *stmt_block(struct parser *parser, int *types, int num_types);
static struct ast *stmt_routine(struct parser *parser);
static struct ast *stmt_while(struct parser *parser);
static struct ast *stmt_if(struct parser *parser);
static struct ast *stmt_expr(struct parser *parser);
static struct ast *stmt(struct parser *parser);
static struct ast *program(struct parser *parser);

struct ast *parser_parse(struct parser *parser)
{
    return expr(parser);
}

static struct ast *expr_number(struct parser *parser)
{
    struct token *token    = consume(parser, (int[]){ TOKEN_NUMBER }, 1); // consume number token
    struct ast   *ast      = malloc(sizeof(struct ast));                  // allocate memory for ast
    ast->type              = AST_EXPR_NUMBER;                             // number ast
    ast->expr.number.value = atof(token->lexeme);                         // set ast value
    return ast;
}

static struct ast *expr_string(struct parser *parser)
{
    struct token *token    = consume(parser, (int[]){ TOKEN_STRING }, 1); // consume string token
    struct ast   *ast      = malloc(sizeof(struct ast));                  // allocate memory for ast
    ast->type              = AST_EXPR_STRING;                             // string ast
    ast->expr.string.value = token->lexeme;                               // set ast value
    return ast;
}

static struct ast *expr_variable(struct parser *parser)
{
    struct token *token = consume(parser, (int[]){ TOKEN_IDENTIFIER }, 1); // consume variable token
    if (check(parser, (int[]){ TOKEN_LPAREN }, 1))
    {
        struct ast *ast         = malloc(sizeof(struct ast)); // allocate memory for ast
        ast->type               = AST_EXPR_CALL;              // call ast
        ast->expr.call.name     = token->lexeme;              // set ast name
        ast->expr.call.num_args = 0;                          // set ast number of arguments
        ast->expr.call.args     = NULL;                       // set ast arguments
        consume(parser, (int[]){ TOKEN_LPAREN }, 1);          // consume left parenthesis token
        while (!check(parser, (int[]){ TOKEN_RPAREN }, 1))
        {
            ast->expr.call.num_args++; // increment number of arguments
            ast->expr.call.args = realloc(
                ast->expr.call.args, ast->expr.call.num_args * sizeof(struct ast *)); // reallocate memory for arguments
            ast->expr.call.args[ast->expr.call.num_args - 1] = expr(parser);          // parse expression
        }
        consume(parser, (int[]){ TOKEN_RPAREN }, 1); // consume right parenthesis token
        return ast;
    }

    struct ast *ast         = malloc(sizeof(struct ast)); // allocate memory for ast
    ast->type               = AST_EXPR_VARIABLE;          // variable ast
    ast->expr.variable.name = token->lexeme;              // set ast name
    return ast;
}

static struct ast *expr_primary(struct parser *parser)
{
    if (check(parser, (int[]){ TOKEN_NUMBER }, 1)) // if current token is number
        return expr_number(parser);                // parse number expression

    if (check(parser, (int[]){ TOKEN_STRING }, 1)) // if current token is string
        return expr_string(parser);                // parse string expression

    if (check(parser, (int[]){ TOKEN_IDENTIFIER }, 1)) // if current token is variable
        return expr_variable(parser);                  // parse variable expression

    if (check(parser, (int[]){ TOKEN_LPAREN }, 1)) // if current token is left parenthesis
    {
        consume(parser, (int[]){ TOKEN_LPAREN }, 1); // consume left parenthesis token
        struct ast *ast = expr(parser);              // parse expression
        consume(parser, (int[]){ TOKEN_RPAREN }, 1); // consume right parenthesis token
        return ast;
    }

    fprintf(stderr, "error: expected expression\n");
    return NULL;
}

static struct ast *expr_unary(struct parser *parser)
{
    if (check(parser, (int[]){ TOKEN_MINUS }, 1)) // if current token is minus
    {
        struct token *token  = consume(parser, (int[]){ TOKEN_MINUS }, 1); // consume minus token
        struct ast   *ast    = malloc(sizeof(struct ast));                 // allocate memory for ast
        ast->type            = AST_EXPR_UNARY;                             // unary ast
        ast->expr.unary.op   = *token;                                     // set ast operator
        ast->expr.unary.expr = expr_unary(parser);                         // set ast right hand side
        return ast;
    }

    return expr_primary(parser);
}

static struct ast *expr_exponent(struct parser *parser)
{
    struct ast *ast = expr_unary(parser); // parse unary expression

    if (check(parser, (int[]){ TOKEN_EXP }, 1)) // while current token is exponent
    {
        struct token *token    = consume(parser, (int[]){ TOKEN_EXP }, 1); // consume exponent token
        struct ast   *rhs      = expr_exponent(parser);                    // right-recurse exponent expression
        struct ast   *lhs      = ast;                                      // save initial ast
        ast                    = malloc(sizeof(struct ast));               // allocate memory for new ast
        ast->type              = AST_EXPR_BINARY;                          // binary ast
        ast->expr.binary.op    = *token;                                   // set ast operator
        ast->expr.binary.left  = lhs;                                      // set ast left hand side
        ast->expr.binary.right = rhs;                                      // set ast right hand side
    }

    return ast;
}

static struct ast *expr_multiplicative(struct parser *parser)
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
        ast->expr.binary.op    = *token;                     // set ast operator
        ast->expr.binary.left  = lhs;                        // set ast left hand side
        ast->expr.binary.right = rhs;                        // set ast right hand side
    }

    return ast;
}

static struct ast *expr_additive(struct parser *parser)
{
    struct ast *ast     = expr_multiplicative(parser); // parse multiplicative expression
    int         types[] = { TOKEN_PLUS, TOKEN_MINUS };

    while (check(parser, types, 2)) // while current token is plus or minus
    {
        struct token *token    = consume(parser, types, 2);   // consume plus or minus token
        struct ast   *rhs      = expr_multiplicative(parser); // parse inner multiplicative expression
        struct ast   *lhs      = ast;                         // save initial ast
        ast                    = malloc(sizeof(struct ast));  // allocate memory for new ast
        ast->type              = AST_EXPR_BINARY;             // binary ast
        ast->expr.binary.op    = *token;                      // set ast operator
        ast->expr.binary.left  = lhs;                         // set ast left hand side
        ast->expr.binary.right = rhs;                         // set ast right hand side
    }

    return ast;
}

static struct ast *expr_relational(struct parser *parser)
{
    struct ast *ast     = expr_additive(parser); // parse additive expression
    int         types[] = { TOKEN_LT, TOKEN_GT, TOKEN_LE, TOKEN_GE };

    while (check(parser, types,
                 4)) // while current token is <, >, <=, or >=
    {
        struct token *token    = consume(parser, types,
                                      4);                 // consume aforementioned tokens
        struct ast   *rhs      = expr_additive(parser);      // parse inner additive expression
        struct ast   *lhs      = ast;                        // save initial ast
        ast                    = malloc(sizeof(struct ast)); // allocate memory for new ast
        ast->type              = AST_EXPR_BINARY;            // binary ast
        ast->expr.binary.op    = *token;                     // set ast operator
        ast->expr.binary.left  = lhs;                        // set ast left hand side
        ast->expr.binary.right = rhs;                        // set ast right hand side
    }

    return ast;
}

static struct ast *expr_equality(struct parser *parser)
{
    struct ast *ast     = expr_relational(parser); // parse relational expression
    int         types[] = { TOKEN_EQ, TOKEN_NE };

    while (check(parser, types, 2)) // while current token is = or <>
    {
        struct token *token    = consume(parser, types, 2);  // consume aforementioned tokens
        struct ast   *rhs      = expr_relational(parser);    // parse inner relational expression
        struct ast   *lhs      = ast;                        // save initial ast
        ast                    = malloc(sizeof(struct ast)); // allocate memory for new ast
        ast->type              = AST_EXPR_BINARY;            // binary ast
        ast->expr.binary.op    = *token;                     // set ast operator
        ast->expr.binary.left  = lhs;                        // set ast left hand side
        ast->expr.binary.right = rhs;                        // set ast right hand side
    }

    return ast;
}

static struct ast *expr_assignment(struct parser *parser)
{
    struct ast *ast = expr_equality(parser); // parse equality expression

    if (check(parser, (int[]){ TOKEN_COLON }, 1)) // if current token is :
    {
        struct token *token    = consume(parser, (int[]){ TOKEN_COLON }, 1); // consume : token
        struct ast   *rhs      = expr_assignment(parser);                    // recursively parse assignment expression
        struct ast   *lhs      = ast;                                        // save initial ast
        ast                    = malloc(sizeof(struct ast));                 // allocate memory for new ast
        ast->type              = AST_EXPR_BINARY;                            // binary ast
        ast->expr.binary.op    = *token;                                     // set ast operator
        ast->expr.binary.left  = lhs;                                        // set ast left hand side
        ast->expr.binary.right = rhs;                                        // set ast right hand side
    }

    return ast;
}

static struct ast *expr(struct parser *parser)
{
    return expr_assignment(parser);
}

static struct ast *program(struct parser *parser)
{
    struct ast *stmts     = malloc(sizeof(struct ast)); // allocate memory for ast
    int         num_stmts = 0;                          // number of statements

    while (!is_at_end(parser)) // while current token is not last token
    {
        struct ast *stmt = stmt_expr(parser);                                  // parse expression statement
        num_stmts++;                                                           // increment number of statements
        stmts                = realloc(stmts, sizeof(struct ast) * num_stmts); // reallocate memory for ast
        stmts[num_stmts - 1] = *stmt;                                          // add statement to ast
    }

    struct ast *node           = malloc(sizeof(struct ast)); // allocate memory for ast
    node->type                 = AST_STMT_BLOCK;             // program ast
    node->stmt.block.num_stmts = num_stmts;                  // set number of statements
    node->stmt.block.stmts     = stmts;                      // set statements

    return node;
}

static struct ast *stmt(struct parser *parser)
{
    if (check(parser, (int[]){ TOKEN_IF }, 1))
        return stmt_if(parser);

    if (check(parser, (int[]){ TOKEN_WHILE }, 1))
        return stmt_while(parser);

    if (check(parser, (int[]){ TOKEN_ROUTINE }, 1))
        return stmt_routine(parser);

    return stmt_expr(parser);
}

static struct ast *stmt_block(struct parser *parser, int *types, int num_types)
{
    struct ast *stmts     = malloc(sizeof(struct ast)); // allocate memory for ast
    int         num_stmts = 0;                          // number of statements

    while (!check(parser, types, num_types) &&
           !is_at_end(parser)) // while current token is not one of the types and is not last token
    {
        struct ast *a = stmt(parser);                                          // parse statement
        num_stmts++;                                                           // increment number of statements
        stmts                = realloc(stmts, sizeof(struct ast) * num_stmts); // reallocate memory for ast
        stmts[num_stmts - 1] = *a;                                             // add statement to ast
    }

    struct ast *node           = malloc(sizeof(struct ast)); // allocate memory for ast
    node->type                 = AST_STMT_BLOCK;             // block ast
    node->stmt.block.num_stmts = num_stmts;                  // set number of statements
    node->stmt.block.stmts     = stmts;                      // set statements

    return node;
}

static struct ast *stmt_if(struct parser *parser)
{
    consume(parser, (int[]){ TOKEN_IF }, 1); // consume if token

    struct ast *cond = expr(parser); // parse condition

    consume(parser, (int[]){ TOKEN_THEN }, 1); // consume then token

    struct ast *then = stmt_block(parser, (int[]){ TOKEN_ELSE, TOKEN_END }, 2); // parse then block

    struct ast *node    = malloc(sizeof(struct ast)); // allocate memory for ast
    node->type          = AST_STMT_IF;                // if else ast
    node->stmt.if_.cond = cond;                       // set condition
    node->stmt.if_.then = then;                       // set then block

    if (check(parser, (int[]){ TOKEN_ELSE }, 1)) // if current token is else
    {
        consume(parser, (int[]){ TOKEN_ELSE }, 1);                          // consume else token
        struct ast *else_    = stmt_block(parser, (int[]){ TOKEN_END }, 1); // parse else block
        node->stmt.if_.else_ = else_;                                       // set else block
    }

    consume(parser, (int[]){ TOKEN_END }, 1); // consume endif token

    return node;
}

static bool is_at_end(struct parser *parser)
{
    return parser->i >= parser->num_tokens; // current token is last token
}

static struct token *peek(struct parser *parser)
{
    return &parser->tokens[parser->i]; // reference to current token
}

static bool check(struct parser *parser, int *types, int num_types)
{
    if (is_at_end(parser))
        return false;

    for (int i = 0; i < num_types; i++)     // for each type
        if (peek(parser)->type == types[i]) // if current token is one of the types
            return true;                    // return true

    return false; // current token is not one of the types
}

static void advance(struct parser *parser)
{
    if (!is_at_end(parser))
        parser->i++;
}

static struct token *consume(struct parser *parser, int *types, int num_types)
{
    if (check(parser, types, num_types)) // if current token is one of the types
    {
        struct token *token = peek(parser); // reference to current token
        advance(parser);                    // advance to next token
        return token;                       // return reference to current token
    }

    return NULL;
}
