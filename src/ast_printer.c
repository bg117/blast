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
    printf("(");
    visit(ast->expr.binary.left, depth + 1);
    printf(" %s ", ast->expr.binary.op.lexeme);
    visit(ast->expr.binary.right, depth + 1);
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
    }
}

void ast_printer(struct ast *ast)
{
    visit(ast, 0);
}
