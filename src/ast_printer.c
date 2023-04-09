#include "ast_printer.h"

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

static void visit(struct ast *ast, int depth);

static void visit_expr_number(struct ast *ast, int depth)
{
    printf("%f", ast->expr.number.value);
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
    printf(".\n");
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
    visit(ast, 0);
}
