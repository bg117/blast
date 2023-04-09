#include "token.h"

#include "key_value_pair.h"

static const struct key_value_pair TOKEN_TYPE_STR[] = {
    { TOKEN_EOF, "EOF" },
    { TOKEN_NUMBER, "<number>" },
    { TOKEN_STRING, "<string>" },
    { TOKEN_IDENTIFIER, "<identifier>" },
    { TOKEN_PLUS, "+" },
    { TOKEN_MINUS, "-" },
    { TOKEN_MUL, "*" },
    { TOKEN_DIV, "/" },
    { TOKEN_MOD, "%" },
    { TOKEN_EXP, "**" },
    { TOKEN_COLON, ":" },
    { TOKEN_EQ, "=" },
    { TOKEN_NE, "<>" },
    { TOKEN_LT, "<" },
    { TOKEN_GT, ">" },
    { TOKEN_LE, "<=" },
    { TOKEN_GE, ">=" },
    { TOKEN_PERIOD, "." },
    { TOKEN_LPAREN, "^(" },
    { TOKEN_RPAREN, "^)" },
    { TOKEN_IF, "if" },
    { TOKEN_THEN, "then" },
    { TOKEN_ELSE, "else" },
    { TOKEN_END, "end" },
    { TOKEN_WHILE, "while" },
    { TOKEN_DO, "do" },
    { TOKEN_ROUTINE, "routine" },
};

const char *token_type_to_string(int type)
{
    for (int i = 0; i < sizeof(TOKEN_TYPE_STR) / sizeof(TOKEN_TYPE_STR[0]); i++)
        if (TOKEN_TYPE_STR[i].key == type)
            return TOKEN_TYPE_STR[i].value;

    return "UNKNOWN";
}
