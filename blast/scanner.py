from .token import Token
from .tokentype import TokenType

class Scanner:
    """Scanner class for lexical analysis of the input stream.
    Uses regular expressions to recognize tokens.
    """

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

        self._tokens.append(Token(TokenType.EOF, "", None, self._line)) # EOF token
        return self._tokens
    
    def _is_at_end(self) -> bool:
        """Checks if the scanner has reached the end of the source code.

        Returns:
            bool: True if the scanner has reached the end of the source code, False otherwise.
        """
        return self._current >= len(self._source) # current index is greater than the length of the source code
    
    def _scan_token(self) -> Token:
        pass