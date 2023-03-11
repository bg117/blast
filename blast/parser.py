from .scanner import Scanner
from .token import Token, TokenType
from .ast import *


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

    def parse(self):
        """Parse the tokens and return the abstract syntax tree.

        Returns:
            AST: The abstract syntax tree.
        """
        return self._expression()
    
    def _match(self, types):
        if self._check(types):
            self._advance()
            return True
        return False
    
    def _check(self, types):
        if self._is_at_end():
            return False
        return self.tokens[self.current].type in types
    
    def _consume(self, types):
        if self._check(types):
            token = self.tokens[self.current]
            self._advance()
            return token
        raise Exception("Unexpected token")
    
    def _is_at_end(self):
        return self.current >= len(self.tokens)
    
    def _advance(self):
        if not self._is_at_end():
            self.current += 1
    
    def _expression(self):
        return self._addition()
    
    def _addition(self):
        expr = self._multiplication()
        types = [TokenType.PLUS, TokenType.MINUS]
        
        while self._match(types):
            operator = self._consume(types)
            right = self._multiplication()
            expr = BinaryExprAST(operator, expr, right)
        
        return expr
    
    def _multiplication(self):
        pass
