#include "ast_printer.h"

#include <stdbool.h>
#include <stdio.h>

#include "ast.h"
#include "token.h"

static void visit_expr_number(struct ast *ast, int depth);
static void visit_expr_string(struct ast *ast, int depth);
static void visit_expr_variable(struct ast *ast, int depth);
static void visit_expr_binary(struct ast *ast, int depth);
static void visit_expr_unary(struct ast *ast, int depth);
static void visit_expr_call(struct ast *ast, int depth);
static void visit_stmt_expr(struct ast *ast, int depth);
static void visit_stmt_block(struct ast *ast, int depth);
static void visit_stmt_if(struct ast *ast, int depth);
static void visit_stmt_while(struct ast *ast, int depth);
static void visit_stmt_routine(struct ast *ast, int depth);

static void visit_error(struct ast *ast, int depth);

static void visit(struct ast *ast, int depth);

// checks if the AST has any errors; if it does, it prints them
// and does not continue printing other parts of the AST
static bool ast_error_precheck(struct ast *ast);

static void visit_expr_number(struct ast *ast, int depth)
{
    printf("%g", ast->expr.number.value);
}

static void visit_expr_string(struct ast *ast, int depth)
{
    printf("\"%s\"", ast->expr.string.value);
}

static void visit_expr_variable(struct ast *ast, int depth)
{
    printf("%s", ast->expr.variable.name);
}

static void visit_expr_binary(struct ast *ast, int depth)
{
    if (ast->expr.binary.left->type == AST_EXPR_BINARY)
        printf("(");

    visit(ast->expr.binary.left, depth + 1);

    if (ast->expr.binary.left->type == AST_EXPR_BINARY)
        printf(")");

    printf(" %s ", ast->expr.binary.op.lexeme);

    if (ast->expr.binary.right->type == AST_EXPR_BINARY)
        printf("(");

    visit(ast->expr.binary.right, depth + 1);

    if (ast->expr.binary.right->type == AST_EXPR_BINARY)
        printf(")");
}

static void visit_expr_unary(struct ast *ast, int depth)
{
    printf("(");
    printf("%s", ast->expr.unary.op.lexeme);
    visit(ast->expr.unary.expr, depth + 1);
    printf(")");
}

static void visit_expr_call(struct ast *ast, int depth)
{
    printf("%s(", ast->expr.call.name);
    for (int i = 0; i < ast->expr.call.num_args; i++)
    {
        visit(ast->expr.call.args[i], depth + 1);
        if (i < ast->expr.call.num_args - 1)
            printf(" ");
    }
    printf(")");
}

static void visit(struct ast *ast, int depth)
{
    switch (ast->type)
    {
    case AST_EXPR_NUMBER:
        visit_expr_number(ast, depth);
        break;
    case AST_EXPR_STRING:
        visit_expr_string(ast, depth);
        break;
    case AST_EXPR_VARIABLE:
        visit_expr_variable(ast, depth);
        break;
    case AST_EXPR_BINARY:
        visit_expr_binary(ast, depth);
        break;
    case AST_EXPR_UNARY:
        visit_expr_unary(ast, depth);
        break;
    case AST_EXPR_CALL:
        visit_expr_call(ast, depth);
        break;
    case AST_STMT_EXPR:
        visit_stmt_expr(ast, depth);
        break;
    case AST_STMT_BLOCK:
        visit_stmt_block(ast, depth);
        break;
    case AST_STMT_IF:
        visit_stmt_if(ast, depth);
        break;
    case AST_STMT_WHILE:
        visit_stmt_while(ast, depth);
        break;
    case AST_STMT_ROUTINE:
        visit_stmt_routine(ast, depth);
        break;
    }
}

static void visit_stmt_expr(struct ast *ast, int depth)
{
    printf("%*s", depth * 4, "");
    visit(ast->stmt.expr.expr, depth + 1);
    printf("\n");
}

static void visit_stmt_block(struct ast *ast, int depth)
{
    for (int i = 0; i < ast->stmt.block.num_stmts; i++)
        visit(ast->stmt.block.stmts[i], depth);
}

static void visit_stmt_if(struct ast *ast, int depth)
{
    printf("%*s", depth * 4, "");
    printf("if ");
    visit(ast->stmt.if_.cond, depth + 1);
    printf(" then\n");
    visit(ast->stmt.if_.then, depth + 1);
    if (ast->stmt.if_.else_)
    {
        printf("%*s", depth * 4, "");
        printf("else\n");
        visit(ast->stmt.if_.else_, depth + 1);
    }

    printf("%*s", depth * 4, "");
    printf("end\n");
}

static void visit_stmt_while(struct ast *ast, int depth)
{
    printf("%*s", depth * 4, "");
    printf("while ");
    visit(ast->stmt.while_.cond, depth + 1);
    printf(" do\n");
    visit(ast->stmt.while_.body, depth + 1);
    printf("%*s", depth * 4, "");
    printf("end\n");
}

static void visit_stmt_routine(struct ast *ast, int depth)
{
    printf("%*s", depth * 4, "");
    printf("routine %s(", ast->stmt.routine.name);
    for (int i = 0; i < ast->stmt.routine.num_params; i++)
    {
        printf("%s", ast->stmt.routine.params[i]);
        if (i < ast->stmt.routine.num_params - 1)
            printf(" ");
    }
    printf(")\n");
    visit(ast->stmt.routine.body, depth + 1);
    printf("%*s", depth * 4, "");
    printf("end\n");
}

void ast_printer(struct ast *ast)
{
    bool error = ast_error_precheck(ast); // check if there are any errors
    if (!error)
        visit(ast, 0);
}

static bool ast_error_precheck(struct ast *ast)
{
    switch (ast->type)
    {
    case AST_ERROR: // if there is an error, print it and return true
        fprintf(stderr, "error: %s\n", ast->error.message);
        return true;
    case AST_EXPR_BINARY: // check children
        return ast_error_precheck(ast->expr.binary.left) || ast_error_precheck(ast->expr.binary.right);
    case AST_EXPR_UNARY:
        return ast_error_precheck(ast->expr.unary.expr);
    case AST_EXPR_CALL: // check arguments
        for (int i = 0; i < ast->expr.call.num_args; i++)
            if (ast_error_precheck(ast->expr.call.args[i]))
                return true;
        return false;
    case AST_STMT_EXPR: // check expression
        return ast_error_precheck(ast->stmt.expr.expr);
    case AST_STMT_BLOCK: // check statements
        for (int i = 0; i < ast->stmt.block.num_stmts; i++)
            if (ast_error_precheck(ast->stmt.block.stmts[i]))
                return true;
        return false;
    case AST_STMT_IF: // check condition, body, else-body
        return ast_error_precheck(ast->stmt.if_.cond) || ast_error_precheck(ast->stmt.if_.then) ||
               ast_error_precheck(ast->stmt.if_.else_);
    case AST_STMT_WHILE: // check condition, body
        return ast_error_precheck(ast->stmt.while_.cond) || ast_error_precheck(ast->stmt.while_.body);
    case AST_STMT_ROUTINE: // check body
        return ast_error_precheck(ast->stmt.routine.body);
    default: // no error
        return false;
    }
}
