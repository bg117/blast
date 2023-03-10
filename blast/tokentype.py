from enum import Enum

class TokenType(Enum):
    EOF = -1

    NUMBER = 0

    PLUS = 1
    MINUS = 2
    MUL = 3
    DIV = 4

    LPAREN = 5
    RPAREN = 6