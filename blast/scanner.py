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
        while self._is_whitespace():
            self._advance()

    def _is_whitespace(self):
        return not self._is_at_end() and self._source[self._current] in " \t\r\n\f\v"
    
    def _scan_token(self):
        self._skip_whitespace()

        if self._is_at_end():
            return # do not do anything if at end of source code
        
        # get the characters until the next whitespace
        # and check if it matches any of the patterns
        s = ''
        while not self._is_at_end() and not self._is_whitespace():
            c = self._source[self._current] # get the current character
            s += c                          # append it to the string

            self._advance()                 # advance to the next character

        # check if the string matches any of the patterns
        self._add_token(s)
    
    def _add_token(self, s):
        # find the token type that matches the string
        for token_type, pattern in self.PATTERNS.items():
            if re.match(pattern, s):
                self._tokens.append(Token(token_type, s))
                return
        
        raise Exception(f"Invalid token: {s}")