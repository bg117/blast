#ifndef PARSER_H
#define PARSER_H

struct ast;
struct token;

/**
 * @brief Struct representing a parser.
 *
 * A parser is a data structure that reads tokens and returns an AST.
 * The parser is a recursive descent parser.
 */
struct parser
{
    struct ast   *root;
    struct token *tokens;

    int i;
};

struct ast *parser_parse(void);

#endif
