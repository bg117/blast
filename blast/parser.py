from .scanner import Scanner
from .token import Token, TokenType
from .ast import *


class Parser:
    """The parser for the BLAST language. This class is used to parse the tokens collected
    by the scanner into an abstract syntax tree.
    """

    def __init__(self, source: str = None, tokens: list[Token] = None):
        """Initialize a Parser using either:
            - source code
            - a list of tokens

        Args:
            source (str?): The source code to parse.
            tokens (list[Token]?): The list of tokens to parse.

        Notes:
            Only one of the arguments MUST be provided. If more than one is provided,
            the Parser will use the first one provided.
        """
        if tokens is not None:
            self._tokens = tokens
        elif source is not None:
            self._tokens = Scanner(source).scan_tokens()
        else:
            raise Exception("No source code or tokens provided.")
        self._current = 0

    def parse(self):
        """Parse the tokens and return the abstract syntax tree.

        Returns:
            AST: The abstract syntax tree.
        """
        return self._expression()
    
    def _check(self, types):
        if self._is_at_end():
            return False
        return self._tokens[self._current].type in types
    
    def _consume(self, types):
        if self._check(types):
            token = self._tokens[self._current]
            self._advance()
            return token
        raise Exception("Unexpected token")
    
    def _is_at_end(self):
        return self._current >= len(self._tokens)
    
    def _advance(self):
        if not self._is_at_end():
            self._current += 1
    
    def _expression(self):
        return self._addition()
    
    def _addition(self):
        expr = self._multiplication()
        types = [TokenType.PLUS, TokenType.MINUS]
        
        while self._check(types):
            operator = self._consume(types)
            right = self._multiplication()
            expr = BinaryExprAST(operator, expr, right)
        
        return expr
    
    def _multiplication(self):
        expr = self._unary()
        types = [TokenType.MUL, TokenType.DIV]

        while self._check(types):
            operator = self._consume(types)
            right = self._unary()
            expr = BinaryExprAST(operator, expr, right)

        return expr
    
    def _unary(self):
        if self._check([TokenType.MINUS]):
            operator = self._consume([TokenType.MINUS])
            right = self._unary()
            return UnaryExprAST(operator, right)
        return self._primary()
    
    def _primary(self):
        if self._check([TokenType.NUMBER]):
            return NumberExprAST(self._consume([TokenType.NUMBER]))
        elif self._check([TokenType.LPAREN]):
            self._consume([TokenType.LPAREN])
            expr = self._expression()
            self._consume([TokenType.RPAREN])
            return expr
        raise Exception("Unexpected token")
