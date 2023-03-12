from .token import TokenType
from .parser import Parser
from .ast import *


class Interpreter:
    """The interpreter for the BLAST programming language.
    """
    def __init__(self, source: str = None, parser: Parser = None, ast: AST = None):
        """Initialize a new Interpreter instance from either:
            - source code
            - a Parser instance
            - an AST instance

        Args:
            source (str?): The source code to interpret.
            parser (Parser?): The Parser instance to interpret.
            ast (AST?): The AST instance to interpret.
        
        Notes:
            Only one of the arguments MUST be provided. If more than one is provided,
            the Interpreter will use the first one provided.
        """
        if ast is not None:
            self.ast = ast
        elif parser is not None:
            self.ast = parser.parse()
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
