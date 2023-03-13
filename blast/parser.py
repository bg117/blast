"""Parser for the BLAST language.

This module contains the Parser class, which is used to parse the tokens collected
by the scanner into an abstract syntax tree.
"""

from .scanner import Scanner
from .token import Token, TokenType
from .ast import *


class Parser:
    """The parser for the BLAST language.
    
    This class is used to parse the tokens collected by the scanner into an abstract
    syntax tree.
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

    def parse(self) -> AST:
        """Parse the tokens and return the abstract syntax tree.

        Returns:
            AST: The abstract syntax tree.
        """
        return self._program()

    def _check(self, types):
        if self._is_at_end():
            return False
        # check if the current token is in the list of types
        return self._tokens[self._current].type in types

    def _consume(self, types):
        if self._check(types):
            token = self._tokens[self._current]  # get the current token
            self._advance()                     # advance the current token
            return token                        # return the previous token
        # if at end, raise an EOF error
        if self._is_at_end():
            raise Exception("Unexpected end of file")
        # otherwise, raise an unexpected token error
        raise Exception(f"Unexpected token: {self._tokens[self._current]}")

    def _is_at_end(self):
        return self._current >= len(self._tokens)

    def _advance(self):
        if not self._is_at_end():
            self._current += 1

    def _expression(self):
        return self._assignment()
    
    def _assignment(self):
        expr = self._addition()
        # right-recursive
        if self._check([TokenType.COLON]):
            operator = self._consume([TokenType.COLON])
            right = self._assignment()
            return BinaryExprAST(operator, expr, right)
        return expr

    def _addition(self):
        expr = self._multiplication()
        types = [TokenType.PLUS, TokenType.MINUS]

        # while the current token is + or -, keep parsing higher precedence expressions
        while self._check(types):
            operator = self._consume(types)
            right = self._multiplication()
            # create a new binary expression with the left and right expressions
            expr = BinaryExprAST(operator, expr, right)

        return expr

    def _multiplication(self):
        expr = self._exponent()
        types = [TokenType.MUL, TokenType.DIV, TokenType.MOD]

        while self._check(types):
            operator = self._consume(types)
            right = self._exponent()
            expr = BinaryExprAST(operator, expr, right)

        return expr
    
    def _exponent(self):
        expr = self._unary()

        # right-recursive; parse the next expression first (which is either an exponent or a unary expression)
        if self._check([TokenType.EXP]):
            operator = self._consume([TokenType.EXP])
            right = self._exponent()
            expr = BinaryExprAST(operator, expr, right)

        return expr

    def _unary(self):
        # currently the only unary operator is -
        if self._check([TokenType.MINUS]):
            operator = self._consume([TokenType.MINUS])  # consume the operator
            # right-recursively parse the next expression
            right = self._unary()
            return UnaryExprAST(operator, right)
        return self._primary()  # if there is no unary operator, parse the next expression

    def _primary(self):
        # if the current token is a number, return a new NumberExprAST
        if self._check([TokenType.NUMBER]):
            # check if convertible to int (assume float if not)
            token = self._consume([TokenType.NUMBER])
            try:
                val = int(token.lexeme)
            except ValueError:
                val = float(token.lexeme)

            return NumberExprAST(val)
        # if the current token is a string, return a new StringExprAST
        elif self._check([TokenType.STRING]):
            return StringExprAST(self._consume([TokenType.STRING]).lexeme)
        # if the current token is an identifier, return a new VariableExprAST
        elif self._check([TokenType.IDENTIFIER]):
            return VariableExprAST(self._consume([TokenType.IDENTIFIER]).lexeme)
        # if the current token is a left parenthesis, parse the next expression
        elif self._check([TokenType.LPAREN]):
            self._consume([TokenType.LPAREN])
            expr = self._expression()
            self._consume([TokenType.RPAREN])
            return expr
        raise Exception(f"Unexpected token: {self._tokens[self._current]}")
    
    def _program(self):
        # store all statements in a list
        statements = []
        # while the current token is not EOF, parse the next statement
        while not self._is_at_end():
            statements.append(self._statement())
        
        return BlockStmtAST(statements)
    
    def _statement(self):
        # if current token is "if", parse an if statement
        if self._check([TokenType.IF]):
            return self._if_statement()
        elif self._check([TokenType.WHILE]):
            return self._while_statement()
        else:
            return self._expression_statement()
    
    def _expression_statement(self):
        stmt = ExprStmtAST(self._expression())
        self._consume([TokenType.PERIOD])

        return stmt
    
    def _if_statement(self):
        self._consume([TokenType.IF])
        condition = self._expression()

        self._consume([TokenType.THEN])
        then_branch = self._block_statement_until([TokenType.ELSE, TokenType.END]) # parse until "else" or "end"
        else_branch = None

        if self._check([TokenType.ELSE]):                               # if there is an else branch, parse it
            self._consume([TokenType.ELSE])                             # consume the "else"
            else_branch = self._block_statement_until([TokenType.END])  # parse until "end"
        
        self._consume([TokenType.END])
        return IfStmtAST(condition, then_branch, else_branch)
    
    def _while_statement(self):
        self._consume([TokenType.WHILE])
        condition = self._expression()
        self._consume([TokenType.DO])
        body = self._block_statement_until([TokenType.END])
        self._consume([TokenType.END])
        return WhileStmtAST(condition, body)
    
    def _block_statement_until(self, types):
        statements = []
        while not self._is_at_end() and not self._check(types):
            statements.append(self._statement())
        if self._is_at_end():
            raise Exception("Unexpected end of file")
        return BlockStmtAST(statements)
