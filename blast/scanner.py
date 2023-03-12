import re
from .token import Token, TokenType


class Scanner:
    """Scanner class for lexical analysis of the input stream.
    Uses regular expressions to recognize tokens.
    """

    PATTERNS = {
        TokenType.NUMBER: r"(\d+)",
        TokenType.STRING: r"\"([^\"]*)\"",
        TokenType.PLUS: r"(\+)",
        TokenType.MINUS: r"(-)",
        TokenType.MUL: r"(\*)",
        TokenType.DIV: r"(/)",
        TokenType.LPAREN: r"(\()",
        TokenType.RPAREN: r"(\))"
    }

    def __init__(self, source: str):
        """Creates a new scanner instance.

        Args:
            source (str): The source code to be scanned.
        """
        self._source = source
        self._tokens = []
        self._current = 0

    def scan_tokens(self) -> list[Token]:
        """Scans the source code and returns a list of tokens.

        Returns:
            list[Token]: A list of tokens.
        """
        # compile all the patterns
        while not self._is_at_end():
            self._scan_token()

        return self._tokens

    def _is_at_end(self):
        # current index is greater than the length of the source code
        return self._current >= len(self._source)

    def _advance(self):
        if self._is_at_end():
            return  # do not do anything if at end of source code

        self._current += 1  # increment current index

    def _skip_whitespace(self):
        while self._source[self._current].isspace():
            self._advance()

    def _scan_token(self):
        self._skip_whitespace()

        # match next token to a pattern in PATTERNS dict
        # if match found, create a token and append to tokens list
        # if no match found, raise an error
        for token_type, pattern in self.PATTERNS.items():
            match = re.match(pattern, self._source[self._current:])
            if match:
                self._tokens.append(Token(token_type, match.group(1)))
                self._current += match.end()
                return

        raise Exception(f"Unexpected character: {self._source[self._current]}")
