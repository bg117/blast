#include "ast_printer.h"

#include <stdio.h>

#include "ast.h"
#include "token.h"

static void visit_expr_number(struct ast *ast, int depth)
{
    printf("%*s%f", depth * 2, " ", ast->expr.number.value);
}

static void visit_expr_string(struct ast *ast, int depth)
{
    printf("%*s\"%s\"", depth * 2, " ", ast->expr.string.value);
}

static void visit_expr_variable(struct ast *ast, int depth)
{
    printf("%*s%s", depth * 2, " ", ast->expr.variable.name);
}

static void visit_expr_binary(struct ast *ast, int depth)
{
    printf("%*s()", depth * 2, " "); // TODO: print nodes
}

static void visit_expr_unary(struct ast *ast, int depth)
{
    printf("%*s()", depth * 2, " "); // TODO: print nodes
}

void ast_printer(struct ast *ast)
{

}
