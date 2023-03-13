"""Token structure and token types.

This module contains the Token class, which represents a token, and the TokenType
enum, which represents the different types of tokens.
"""

from enum import IntEnum, auto


class Token:
    """Class representing a token.

    A token is a lexical unit of a programming language. It is a sequence of characters
    that have a meaning in the programming language. For example, the token '1' is a
    NUMBER token, and the token '+' is a PLUS token.
    """

    def __init__(self, type, lexeme):
        """Creates a new token.

        Args:
            type (TokenType): The type of the token.
            lexeme (Any): The lexeme of the token.

        Notes:
            Technically, the lexeme can be any type, but it is usually a string,
            even for tokens like NUMBER. This is to prevent the need for type
            checking in the parser.
        """
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
    EOF = -1            # end of file

    NUMBER = auto()     # integer
    STRING = auto()     # string
    IDENTIFIER = auto() # variable name

    PLUS = auto()       # plus (+)
    MINUS = auto()      # minus (-)
    MUL = auto()        # multiplication (*)
    DIV = auto()        # division (/)
    MOD = auto()        # modulo (%)
    EXP = auto()        # exponentiation (**)

    COLON = auto()      # colon (:) for assignment

    EQ = auto()         # equal to (=)
    NE = auto()         # not equal to (<>)
    LT = auto()         # less than (<)
    LE = auto()         # less than or equal to (<=)
    GT = auto()         # greater than (>)
    GE = auto()         # greater than or equal to (>=)

    PERIOD = auto()     # period (.) for ending a statement

    LPAREN = auto()     # left parenthesis (
    RPAREN = auto()     # right parenthesis )

    IF = auto()         # if keyword
    THEN = auto()       # then keyword
    ELSE = auto()       # else keyword
    END = auto()        # end keyword

    WHILE = auto()      # while keyword
    DO = auto()         # do keyword

    ROUTINE = auto()    # routine keyword (for defining a function)
