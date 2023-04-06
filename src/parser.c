#include "parser.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "token.h"

#define CONS(...) (int[]){ __VA_ARGS__ }, sizeof((int[]){ __VA_ARGS__ }) / sizeof(int)

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
    struct token *token    = consume(parser, CONS(TOKEN_NUMBER)); // consume number token
    struct ast   *ast      = malloc(sizeof(struct ast));          // allocate memory for ast
    ast->type              = AST_EXPR_NUMBER;                     // number ast
    ast->expr.number.value = atof(token->lexeme);                 // set ast value
    return ast;
}

static struct ast *expr_string(struct parser *parser)
{
    struct token *token    = consume(parser, CONS(TOKEN_STRING)); // consume string token
    struct ast   *ast      = malloc(sizeof(struct ast));          // allocate memory for ast
    ast->type              = AST_EXPR_STRING;                     // string ast
    ast->expr.string.value = token->lexeme;                       // set ast value
    return ast;
}

static struct ast *expr_variable(struct parser *parser)
{
    struct token *token = consume(parser, CONS(TOKEN_IDENTIFIER)); // consume variable token
    if (check(parser, CONS(TOKEN_LPAREN)))
    {
        struct ast *ast         = malloc(sizeof(struct ast)); // allocate memory for ast
        ast->type               = AST_EXPR_CALL;              // call ast
        ast->expr.call.name     = token->lexeme;              // set ast name
        ast->expr.call.num_args = 0;                          // set ast number of arguments
        ast->expr.call.args     = NULL;                       // set ast arguments
        consume(parser, CONS(TOKEN_LPAREN));                  // consume left parenthesis token
        while (!check(parser, CONS(TOKEN_RPAREN)))
        {
            ast->expr.call.num_args++; // increment number of arguments
            ast->expr.call.args = realloc(
                ast->expr.call.args, ast->expr.call.num_args * sizeof(struct ast *)); // reallocate memory for arguments
            ast->expr.call.args[ast->expr.call.num_args - 1] = expr(parser);          // parse expression
        }
        consume(parser, CONS(TOKEN_RPAREN)); // consume right parenthesis token
        return ast;
    }

    struct ast *ast         = malloc(sizeof(struct ast)); // allocate memory for ast
    ast->type               = AST_EXPR_VARIABLE;          // variable ast
    ast->expr.variable.name = token->lexeme;              // set ast name
    return ast;
}

static struct ast *expr_primary(struct parser *parser)
{
    if (check(parser, CONS(TOKEN_NUMBER))) // if current token is number
        return expr_number(parser);        // parse number expression

    if (check(parser, CONS(TOKEN_STRING))) // if current token is string
        return expr_string(parser);        // parse string expression

    if (check(parser, CONS(TOKEN_IDENTIFIER))) // if current token is variable
        return expr_variable(parser);          // parse variable expression

    if (check(parser, CONS(TOKEN_LPAREN))) // if current token is left parenthesis
    {
        consume(parser, CONS(TOKEN_LPAREN)); // consume left parenthesis token
        struct ast *ast = expr(parser);      // parse expression
        consume(parser, CONS(TOKEN_RPAREN)); // consume right parenthesis token
        return ast;
    }

    fprintf(stderr, "error: expected expression\n");
    return NULL;
}

static struct ast *expr_unary(struct parser *parser)
{
    if (check(parser, CONS(TOKEN_MINUS))) // if current token is minus
    {
        struct token *token  = consume(parser, CONS(TOKEN_MINUS)); // consume minus token
        struct ast   *ast    = malloc(sizeof(struct ast));         // allocate memory for ast
        ast->type            = AST_EXPR_UNARY;                     // unary ast
        ast->expr.unary.op   = *token;                             // set ast operator
        ast->expr.unary.expr = expr_unary(parser);                 // set ast right hand side
        return ast;
    }

    return expr_primary(parser);
}

static struct ast *expr_exponent(struct parser *parser)
{
    struct ast *ast = expr_unary(parser); // parse unary expression

    if (check(parser, CONS(TOKEN_EXP))) // while current token is exponent
    {
        struct token *token    = consume(parser, CONS(TOKEN_EXP)); // consume exponent token
        struct ast   *rhs      = expr_exponent(parser);            // right-recurse exponent expression
        struct ast   *lhs      = ast;                              // save initial ast
        ast                    = malloc(sizeof(struct ast));       // allocate memory for new ast
        ast->type              = AST_EXPR_BINARY;                  // binary ast
        ast->expr.binary.op    = *token;                           // set ast operator
        ast->expr.binary.left  = lhs;                              // set ast left hand side
        ast->expr.binary.right = rhs;                              // set ast right hand side
    }

    return ast;
}

static struct ast *expr_multiplicative(struct parser *parser)
{
    struct ast *ast = expr_exponent(parser); // parse exponent expression

    while (check(parser, CONS(TOKEN_MUL, TOKEN_DIV))) // while current token is star or slash
    {
        struct token *token    = consume(parser, CONS(TOKEN_MUL, TOKEN_DIV)); // consume star or slash token
        struct ast   *rhs      = expr_exponent(parser);                       // parse inner exponent expression
        struct ast   *lhs      = ast;                                         // save initial ast
        ast                    = malloc(sizeof(struct ast));                  // allocate memory for new ast
        ast->type              = AST_EXPR_BINARY;                             // binary ast
        ast->expr.binary.op    = *token;                                      // set ast operator
        ast->expr.binary.left  = lhs;                                         // set ast left hand side
        ast->expr.binary.right = rhs;                                         // set ast right hand side
    }

    return ast;
}

static struct ast *expr_additive(struct parser *parser)
{
    struct ast *ast = expr_multiplicative(parser); // parse multiplicative expression

    while (check(parser, CONS(TOKEN_PLUS, TOKEN_MINUS))) // while current token is plus or minus
    {
        struct token *token    = consume(parser, CONS(TOKEN_PLUS, TOKEN_MINUS)); // consume plus or minus token
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
    struct ast *ast = expr_additive(parser); // parse additive expression

    while (check(parser, CONS(TOKEN_LT, TOKEN_GT, TOKEN_LE, TOKEN_GE))) // while current token is <, >, <=, or >=
    {
        struct token *token =
            consume(parser, CONS(TOKEN_LT, TOKEN_GT, TOKEN_LE, TOKEN_GE)); // consume aforementioned tokens
        struct ast *rhs        = expr_additive(parser);                    // parse inner additive expression
        struct ast *lhs        = ast;                                      // save initial ast
        ast                    = malloc(sizeof(struct ast));               // allocate memory for new ast
        ast->type              = AST_EXPR_BINARY;                          // binary ast
        ast->expr.binary.op    = *token;                                   // set ast operator
        ast->expr.binary.left  = lhs;                                      // set ast left hand side
        ast->expr.binary.right = rhs;                                      // set ast right hand side
    }

    return ast;
}

static struct ast *expr_equality(struct parser *parser)
{
    struct ast *ast = expr_relational(parser); // parse relational expression

    while (check(parser, CONS(TOKEN_EQ, TOKEN_NE))) // while current token is = or <>
    {
        struct token *token    = consume(parser, CONS(TOKEN_EQ, TOKEN_NE)); // consume aforementioned tokens
        struct ast   *rhs      = expr_relational(parser);                   // parse inner relational expression
        struct ast   *lhs      = ast;                                       // save initial ast
        ast                    = malloc(sizeof(struct ast));                // allocate memory for new ast
        ast->type              = AST_EXPR_BINARY;                           // binary ast
        ast->expr.binary.op    = *token;                                    // set ast operator
        ast->expr.binary.left  = lhs;                                       // set ast left hand side
        ast->expr.binary.right = rhs;                                       // set ast right hand side
    }

    return ast;
}

static struct ast *expr_assignment(struct parser *parser)
{
    struct ast *ast = expr_equality(parser); // parse equality expression

    if (check(parser, CONS(TOKEN_COLON))) // if current token is :
    {
        struct token *token    = consume(parser, CONS(TOKEN_COLON)); // consume : token
        struct ast   *rhs      = expr_assignment(parser);            // recursively parse assignment expression
        struct ast   *lhs      = ast;                                // save initial ast
        ast                    = malloc(sizeof(struct ast));         // allocate memory for new ast
        ast->type              = AST_EXPR_BINARY;                    // binary ast
        ast->expr.binary.op    = *token;                             // set ast operator
        ast->expr.binary.left  = lhs;                                // set ast left hand side
        ast->expr.binary.right = rhs;                                // set ast right hand side
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
    if (check(parser, CONS(TOKEN_IF)))
        return stmt_if(parser);

    if (check(parser, CONS(TOKEN_WHILE)))
        return stmt_while(parser);

    if (check(parser, CONS(TOKEN_ROUTINE)))
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
    consume(parser, CONS(TOKEN_IF)); // consume if token

    struct ast *cond = expr(parser); // parse condition

    consume(parser, CONS(TOKEN_THEN)); // consume then token

    struct ast *then = stmt_block(parser, CONS(TOKEN_ELSE, TOKEN_END)); // parse then block, either stop at else or end

    struct ast *node    = malloc(sizeof(struct ast)); // allocate memory for ast
    node->type          = AST_STMT_IF;                // if else ast
    node->stmt.if_.cond = cond;                       // set condition
    node->stmt.if_.then = then;                       // set then block

    if (check(parser, CONS(TOKEN_ELSE))) // if current token is else
    {
        consume(parser, CONS(TOKEN_ELSE));                          // consume else token
        struct ast *else_    = stmt_block(parser, CONS(TOKEN_END)); // parse else block
        node->stmt.if_.else_ = else_;                               // set else block
    }

    consume(parser, CONS(TOKEN_END)); // consume end token

    return node;
}

static struct ast *stmt_while(struct parser *parser)
{
    consume(parser, CONS(TOKEN_WHILE)); // consume while token

    struct ast *cond = expr(parser); // parse condition

    consume(parser, CONS(TOKEN_DO)); // consume do token

    struct ast *body = stmt_block(parser, CONS(TOKEN_END)); // parse body block, stop at end

    consume(parser, CONS(TOKEN_END)); // consume endwhile token

    struct ast *node       = malloc(sizeof(struct ast)); // allocate memory for ast
    node->type             = AST_STMT_WHILE;             // while ast
    node->stmt.while_.cond = cond;                       // set condition
    node->stmt.while_.body = body;                       // set body block

    return node;
}

static struct ast *stmt_routine(struct parser *parser)
{
    consume(parser, CONS(TOKEN_ROUTINE)); // consume routine token

    struct token *name = consume(parser, CONS(TOKEN_IDENTIFIER)); // consume identifier token

    consume(parser, CONS(TOKEN_LPAREN)); // consume ( token

    char *params     = malloc(sizeof(char)); // allocate memory for ast
    int   num_params = 0;                    // number of parameters

    while (!check(parser, CONS(TOKEN_RPAREN))) // while current token is not )
    {
        struct token *param = consume(parser, CONS(TOKEN_IDENTIFIER));       // consume identifier token
        num_params++;                                                        // increment number of parameters
        params                 = realloc(params, sizeof(char) * num_params); // reallocate memory for params
        params[num_params - 1] = param->lexeme;                              // add parameter to list
    }

    consume(parser, CONS(TOKEN_RPAREN)); // consume ) token

    struct ast *body = stmt_block(parser, CONS(TOKEN_END)); // parse body block, stop at end

    consume(parser, CONS(TOKEN_END)); // consume end token

    struct ast *node              = malloc(sizeof(struct ast)); // allocate memory for ast
    node->type                    = AST_STMT_ROUTINE;           // routine ast
    node->stmt.routine.name       = name;                       // set name
    node->stmt.routine.num_params = num_params;                 // set number of parameters
    node->stmt.routine.params     = params;                     // set parameters
    node->stmt.routine.body       = body;                       // set body block

    return node;
}

static struct ast *stmt_expr(struct parser *parser)
{
    struct ast *a = expr(parser); // parse expression

    consume(parser, CONS(TOKEN_PERIOD)); // consume . token

    struct ast *node     = malloc(sizeof(struct ast)); // allocate memory for ast
    node->type           = AST_STMT_EXPR;              // expression ast
    node->stmt.expr.expr = a;                          // set expression

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
