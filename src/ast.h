#ifndef AST_H
#define AST_H

#include "token.h"

/**
 * @brief Enum representing the different types of AST nodes.
 */
enum ast_type
{
    AST_EXPR_BINARY,
    AST_EXPR_UNARY,
    AST_EXPR_NUMBER,
    AST_EXPR_STRING,
    AST_EXPR_VARIABLE,
    AST_EXPR_CALL,

    AST_STMT_EXPR,
    AST_STMT_BLOCK,
    AST_STMT_IF,
    AST_STMT_WHILE,
    AST_STMT_ROUTINE
};

/**
 * @brief Struct representing a binary expression.
 *
 * A binary expression is an expression that consists of two operands and an operator.
 */
struct ast_expr_binary
{
    struct token op;
    struct ast  *left;
    struct ast  *right;
};

/**
 * @brief Struct representing a unary expression.
 *
 * A unary expression is an expression that consists of one operand and an operator.
 */
struct ast_expr_unary
{
    struct token op;
    struct ast  *expr;
};

/**
 * @brief Struct representing a number expression.
 */
struct ast_expr_number
{
    double value;
};

/**
 * @brief Struct representing a string expression.
 */
struct ast_expr_string
{
    char *value;
};

/**
 * @brief Struct representing a variable reference.
 */
struct ast_expr_variable
{
    char *name;
};

/**
 * @brief Struct representing a routine call.
 */
struct ast_expr_call
{
    char        *name;
    struct ast **args;
    int          num_args;
};

/**
 * @brief Struct representing an expression statement.
 */
struct ast_stmt_expr
{
    struct ast *expr;
};

/**
 * @brief Struct representing a block statement.
 */
struct ast_stmt_block
{
    struct ast **stmts;
    int          num_stmts;
};

/**
 * @brief Struct representing an if statement.
 */
struct ast_stmt_if
{
    struct ast *cond;
    struct ast *then;
    struct ast *else_;
};

/**
 * @brief Struct representing a while statement.
 */
struct ast_stmt_while
{
    struct ast *cond;
    struct ast *body;
};

/**
 * @brief Struct representing a routine definition.
 */
struct ast_stmt_routine
{
    char       *name;
    char      **params;
    int         num_params;
    struct ast *body;
};

/**
 * @brief Struct representing an AST node.
 *
 * An AST node is a node in an abstract syntax tree. An abstract syntax tree is a
 * representation of the syntax of a programming language. It is a tree data structure
 * where each node represents a syntactic construct of the programming language.
 */
struct ast
{
    int type;
    union {
        union {
            struct ast_expr_binary   binary;
            struct ast_expr_unary    unary;
            struct ast_expr_number   number;
            struct ast_expr_string   string;
            struct ast_expr_variable variable;
            struct ast_expr_call     call;
        } expr;
        union {
            struct ast_stmt_expr    expr;
            struct ast_stmt_block   block;
            struct ast_stmt_if      if_;
            struct ast_stmt_while   while_;
            struct ast_stmt_routine routine;
        } stmt;
    };
};

#endif
