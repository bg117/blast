from enum import IntEnum, auto


class Token:
    """Class representing a token.

    A token is a lexical unit of a programming language. It is a sequence of characters
    that have a meaning in the programming language. For example, the token '1' is a
    NUMBER token, and the token '+' is a PLUS token.
    """

    def __init__(self, type, lexeme):
        self.type = type
        self.lexeme = lexeme

    def __repr__(self):
        return f"<{self.type}: {self.lexeme}>"

    def __str__(self):
        return self.lexeme

    def __eq__(self, other):
        return self.type == other.type and self.lexeme == other.lexeme

    def __hash__(self):
        return hash((self.type, self.lexeme))


class TokenType(IntEnum):
    """Enum representing the different types of tokens.
    """
    EOF = -1

    NUMBER = auto()
    STRING = auto()
    IDENTIFIER = auto()

    PLUS = auto()
    MINUS = auto()
    MUL = auto()
    DIV = auto()
    MOD = auto()
    EXP = auto()

    LPAREN = auto()
    RPAREN = auto()
