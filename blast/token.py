from enum import Enum


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


class TokenType(Enum):
    """Enum representing the different types of tokens.
    """
    EOF = -1

    NUMBER = 0

    PLUS = 1
    MINUS = 2
    MUL = 3
    DIV = 4

    LPAREN = 5
    RPAREN = 6
