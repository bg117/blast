from .scanner import Scanner
from .token import Token


class Parser:
    """The parser for the BLAST language. This class is used to parse the tokens collected
    by the scanner into an abstract syntax tree.
    """

    def __init__(self, source: str):
        """Initialize a Parser using the given source code.

        Args:
            source (str): The source code to parse.
        """
        scanner = Scanner(source)
        self.tokens = scanner.scan_tokens()
        self.current = 0

    def __init__(self, tokens: list[Token]):
        """Initialize a Parser using the given list of tokens.

        Args:
            tokens (list): The list of tokens to parse.
        """
        self.tokens = tokens
        self.current = 0