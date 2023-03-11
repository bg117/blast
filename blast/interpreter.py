from .token import TokenType
from .parser import Parser
from .ast import *


class Interpreter:
    """The interpreter for the BLAST programming language.
    """
    def __init__(self, source: str):
        """Initialize a new Interpreter instance from the given source code.

        Args:
            source (str): The source code to interpret.
        """
        parser = Parser(source)
        self.ast = parser.parse()

    def __init__(self, parser: Parser):
        """Initialize a new Interpreter instance from the given parser.

        Args:
            parser (Parser): The parser to use.
        """
        self.ast = parser.parse()
    
    def __init__(self, ast: AST):
        """Initialize a new Interpreter instance from the given AST.

        Args:
            ast (AST): The AST to interpret.
        """
        self.ast = ast

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
        return expr.val.lexeme
