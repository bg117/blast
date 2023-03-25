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
    struct token *tokens;
    int           num_tokens;

    int i;
};

struct ast *parser_parse(struct parser *parser);

#endif
