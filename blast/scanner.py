from .token import Token
from .tokentype import TokenType

class Scanner:
    """Scanner class for lexical analysis of the input stream.
    Uses regular expressions to recognize tokens.
    """

    PATTERNS = {
        TokenType.NUMBER: r"\d+(\.\d+)?",
        TokenType.PLUS: r"\+",
        TokenType.MINUS: r"-",
        TokenType.MUL: r"\*",
        TokenType.DIV: r"/",
        TokenType.LPAREN: r"\(",
        TokenType.RPAREN: r"\)",
    }

    def __init__(self, source: str):
        """Creates a new scanner instance.

        Args:
            source (str): The source code to be scanned.
        """
        self._source = source
        self._tokens = []
        self._start = 0
        self._current = 0
        self._line = 1

    def scan_tokens(self) -> list[Token]:
        """Scans the source code and returns a list of tokens.

        Returns:
            list[Token]: A list of tokens.
        """
        while not self._is_at_end():    # while not EOF
            self._start = self._current # mark the beginning of the next lexeme
            self._scan_token()          # scan the next token

        self._tokens.append(Token(TokenType.EOF, None)) # EOF token
        return self._tokens
    
    def _is_at_end(self):
        return self._current >= len(self._source) # current index is greater than the length of the source code
    
    def _advance(self):
        if self._is_at_end():
            return # do not do anything if at end of source code

        self._current += 1 # increment current index
    
    def _skip_whitespace(self):
        while not self._is_at_end() and self._source[self._current] in " \t\r\n\f\v":
            self._advance()
    
    def _scan_token(self):
        self._skip_whitespace()

        if self._is_at_end():
            return # do not do anything if at end of source code