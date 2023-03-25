#include "parser.h"

#include <stdlib.h>

#include "ast.h"
#include "token.h"

struct ast *parser_parse(void)
{
    struct ast *root = malloc(sizeof(struct ast));
    return root;
}
