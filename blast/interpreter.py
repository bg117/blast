from .token import Token, TokenType
from .parser import Parser
from .ast import *


class Interpreter:
    """The interpreter for the BLAST programming language.
    """

    def __init__(self, source: str = None, tokens: list[Token] = None, ast: AST = None):
        """Initialize a new Interpreter instance from either:
            - source code
            - a list of tokens
            - an abstract syntax tree

        Args:
            source (str?): The source code to interpret.
            tokens (list[Token]?): The list of tokens to interpret.
            ast (AST?): The abstract syntax tree to interpret.

        Notes:
            Only one of the arguments MUST be provided. If more than one is provided,
            the Interpreter will use the first one provided.
        """
        if ast is not None:
            self.ast = ast
        elif tokens is not None:
            self.ast = Parser(tokens=tokens).parse()
        elif source is not None:
            self.ast = Parser(source=source).parse()
        else:
            raise Exception("No source code, parser, or AST provided.")

    def evaluate(self):
        """Interpret the AST and return the result.
        """
        return self.ast.accept(self)

    def visit_binary_expr(self, expr: BinaryExprAST):
        if expr.op.type == TokenType.PLUS:
            return expr.lhs.accept(self) + expr.rhs.accept(self)
        elif expr.op.type == TokenType.MINUS:
            return expr.lhs.accept(self) - expr.rhs.accept(self)
        elif expr.op.type == TokenType.MUL:
            return expr.lhs.accept(self) * expr.rhs.accept(self)
        elif expr.op.type == TokenType.DIV:
            return expr.lhs.accept(self) / expr.rhs.accept(self)

    def visit_unary_expr(self, expr: UnaryExprAST):
        accept = expr.expr.accept(self)
        if expr.op.type == TokenType.MINUS:
            return -accept
        return accept

    def visit_number_expr(self, expr: NumberExprAST):
        return expr.val

    def visit_string_expr(self, expr: StringExprAST):
        return expr.val
