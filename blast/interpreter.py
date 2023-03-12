"""The interpreter for the BLAST programming language.

This module contains the Interpreter class, which is used to interpret a BLAST program.
"""

from .token import Token, TokenType
from .parser import Parser
from .ast import *
from .symtab import SymbolTable


class Interpreter:
    """The interpreter for the BLAST programming language."""

    def __init__(self):
        """Initialize the Interpreter."""

        self._symtab = SymbolTable()

    def evaluate(self, source: str = None, tokens: list[Token] = None, ast: AST = None):
        """Interpret the source code, tokens, or AST and return the result.

        Args:
            source (str?): The source code to interpret.
            tokens (list[Token]?): The list of tokens to interpret.
            ast (AST?): The abstract syntax tree to interpret.

        Notes:
            Only one of the arguments MUST be provided. If more than one is provided,
            the Interpreter will use the first one provided.
        """
        if ast is not None:
            self._ast = ast
        elif tokens is not None:
            self._ast = Parser(tokens=tokens).parse()
        elif source is not None:
            self._ast = Parser(source=source).parse()
        else:
            raise Exception("No source code, parser, or AST provided.")

        return self._ast.accept(self)

    def visit_binary_expr(self, expr: BinaryExprAST):
        rhs = expr.rhs.accept(self)

        # if assignment, add to symbol table
        if expr.op.type == TokenType.COLON:
            if not isinstance(expr.lhs, VariableExprAST):
                raise Exception("Invalid assignment target")
            self._symtab[expr.lhs.name] = rhs
            return

        lhs = expr.lhs.accept(self)

        match expr.op.type:
            case TokenType.PLUS:
                return lhs + rhs
            case TokenType.MINUS:
                return lhs - rhs
            case TokenType.MUL:
                return lhs * rhs    
            case TokenType.DIV:
                return lhs / rhs
            case TokenType.MOD:
                return lhs % rhs
            case TokenType.EXP:
                return lhs ** rhs

    def visit_unary_expr(self, expr: UnaryExprAST):
        accept = expr.expr.accept(self)
        if expr.op.type == TokenType.MINUS:
            return -accept
        return accept

    def visit_number_expr(self, expr: NumberExprAST):
        return expr.val

    def visit_string_expr(self, expr: StringExprAST):
        return expr.val
    
    def visit_variable_expr(self, expr: VariableExprAST):
        try:
            return self._symtab[expr.name]
        except KeyError:
            raise Exception(f"Undefined variable '{expr.name}'")
        
    def visit_expr_stmt(self, stmt: ExprStmtAST):
        return stmt.expr.accept(self)
