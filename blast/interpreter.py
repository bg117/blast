"""The interpreter for the BLAST programming language.

This module contains the Interpreter class, which is used to interpret a BLAST program.
"""

from .token import Token, TokenType
from .parser import Parser
from .ast import *
from .symtab import SymbolTable, SymbolType


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
            self._symtab.set(expr.lhs.name, SymbolType.VARIABLE, rhs)
            return  # return nothing

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
            case TokenType.EQ:
                return lhs == rhs
            case TokenType.NE:
                return lhs != rhs
            case TokenType.LT:
                return lhs < rhs
            case TokenType.LE:
                return lhs <= rhs
            case TokenType.GT:
                return lhs > rhs
            case TokenType.GE:
                return lhs >= rhs

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
            return self._symtab.get(expr.name, SymbolType.VARIABLE)
        except KeyError:
            raise Exception(f"Undefined variable '{expr.name}'")

    def visit_call_expr(self, expr: CallExprAST):
        try:
            # if print function, print the args
            if expr.name == "print":
                print(*[arg.accept(self) for arg in expr.args], sep=" ")
                return  # return nothing

            func = self._symtab.get(expr.name, SymbolType.FUNCTION)
            # create new symbol table for function
            symtab = SymbolTable(parent=self._symtab)
            # func.args is a list of str (names of args); map to expr.args
            # but first, check if the number of args is correct
            if len(func.args) != len(expr.args):
                raise Exception(
                    f"Invalid number of arguments for function '{expr.name}': expected {len(func.args)}, got {len(expr.args)}")
            # now, map the args
            for arg, val in zip(func.args, expr.args):
                symtab.set(arg, SymbolType.VARIABLE, val.accept(self))

            # set the symbol table to the new one
            old_symtab = self._symtab
            self._symtab = symtab

            # evaluate the function
            result = func.body.accept(self)

            # set the symbol table back to the old one
            self._symtab = old_symtab

            return result
        except KeyError:
            raise Exception(f"Undefined function '{expr.name}'")

    def visit_expr_stmt(self, stmt: ExprStmtAST):
        return stmt.expr.accept(self)

    def visit_block_stmt(self, stmt: BlockStmtAST):
        results = []
        for statement in stmt.stmts:
            accept = statement.accept(self)
            # only append if it returns something (not None or bool)
            if accept is bool or accept is not None:
                results.append(accept)

        return results

    def visit_if_stmt(self, stmt: IfStmtAST):
        if stmt.cond.accept(self):
            return stmt.then_block.accept(self)
        elif stmt.else_block:
            return stmt.else_block.accept(self)

    def visit_while_stmt(self, stmt: WhileStmtAST):
        results = []
        while stmt.cond.accept(self):
            accept = stmt.body.accept(self)
            if accept is bool or accept is not None:
                results.append(accept)

        return results

    def visit_routine_stmt(self, stmt: RoutineStmtAST):
        self._symtab.set(stmt.name, SymbolType.FUNCTION, stmt)
