#include "ast_printer.h"

#include <stdio.h>

#include "ast.h"
#include "token.h"

static void visit_expr_number(struct ast *ast, int depth)
{
    printf("%*s%f", depth * 2, " ", ast->expr.number.value);
}

void ast_printer(struct ast *ast)
{

}
