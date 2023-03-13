"""Abstract syntax tree for the BLAST language.

This module contains the classes representing the abstract syntax tree of a BLAST
program.  It is used by the parser and the interpreter.
"""

from abc import ABC


class AST(ABC):
    """Base class for all AST nodes.

    This class is used to represent the abstract syntax tree of a BLAST
    program.  It is used by the parser and the interpreter.
    """

    def __repr__(self):
        raise NotImplementedError("Cannot repr a base AST node")

    def accept(self, visitor):
        """Accept a visitor.

        Args:
            visitor (Any): The visitor to accept.

        Raises:
            NotImplementedError: The base AST node cannot accept a visitor.
        """
        raise NotImplementedError("Cannot accept from a base AST node")


class ExprAST(AST):
    """Base class for all expression AST nodes.
    """
    pass


class BinaryExprAST(ExprAST):
    """AST node representing a binary expression.
    """

    def __init__(self, op, lhs, rhs):
        """Initialize a BinaryExprAST.

        Args:
            op (Token): The operator.
            lhs (ExprAST): The left-hand side expression.
            rhs (ExprAST): The right-hand side expression.
        """
        self.op = op
        self.lhs = lhs
        self.rhs = rhs

    def __repr__(self):
        return f"<{self.op!r} {self.lhs!r} {self.rhs!r}>"

    def accept(self, visitor):
        """Accept a visitor.

        Args:
            visitor (Any): The visitor to accept.

        Returns:
            Any: The result of the visitor's visit_binary_expr method.
        """
        return visitor.visit_binary_expr(self)


class UnaryExprAST(ExprAST):
    """AST node representing a unary expression.
    """

    def __init__(self, op, expr):
        """Initialize a UnaryExprAST.

        Args:
            op (Token): The operator.
            expr (ExprAST): The expression.
        """
        self.op = op
        self.expr = expr

    def __repr__(self):
        return f"<{self.op!r} {self.expr!r}>"

    def accept(self, visitor):
        """Accept a visitor.

        Args:
            visitor (Any): The visitor to accept.

        Returns:
            Any: The result of the visitor's visit_unary_expr method.
        """
        return visitor.visit_unary_expr(self)


class NumberExprAST(ExprAST):
    """AST node representing a number.
    """

    def __init__(self, val):
        """Initialize a NumberExprAST.

        Args:
            val (int): The value of the number.
        """
        self.val = val

    def __repr__(self):
        return f"<{self.val!r}>"

    def accept(self, visitor):
        """Accept a visitor.

        Args:
            visitor (Any): The visitor to accept.

        Returns:
            Any: The result of the visitor's visit_number_expr method.
        """
        return visitor.visit_number_expr(self)


class StringExprAST(ExprAST):
    """AST node representing a string.
    """

    def __init__(self, val):
        """Initialize a StringExprAST.

        Args:
            val (str): The value of the string.
        """
        self.val = val

    def __repr__(self):
        return f"<{self.val!r}>"

    def accept(self, visitor):
        """Accept a visitor.

        Args:
            visitor (Any): The visitor to accept.

        Returns:
            Any: The result of the visitor's visit_string_expr method.
        """
        return visitor.visit_string_expr(self)
    

class VariableExprAST(ExprAST):
    """AST node representing a variable.
    """

    def __init__(self, name):
        """Initialize a VariableExprAST.

        Args:
            name (str): The name of the variable.
        """
        self.name = name

    def __repr__(self):
        return f"<{self.name!r}>"

    def accept(self, visitor):
        """Accept a visitor.

        Args:
            visitor (Any): The visitor to accept.

        Returns:
            Any: The result of the visitor's visit_variable_expr method.
        """
        return visitor.visit_variable_expr(self)
    

class StmtAST(AST):
    """Base class for all statement AST nodes.
    """
    pass


class ExprStmtAST(StmtAST):
    """AST node representing an expression statement.
    """

    def __init__(self, expr):
        """Initialize an ExprStmtAST.

        Args:
            expr (ExprAST): The expression.
        """
        self.expr = expr

    def __repr__(self):
        return f"<{self.expr!r}>"

    def accept(self, visitor):
        """Accept a visitor.

        Args:
            visitor (Any): The visitor to accept.

        Returns:
            Any: The result of the visitor's visit_expr_stmt method.
        """
        return visitor.visit_expr_stmt(self)


class BlockStmtAST(StmtAST):
    """AST node representing a block statement.
    """

    def __init__(self, stmts):
        """Initialize a BlockStmtAST.

        Args:
            stmts (list[StmtAST]): The statements in the block.
        """
        self.stmts = stmts

    def __repr__(self):
        return f"<{self.stmts!r}>"

    def accept(self, visitor):
        """Accept a visitor.

        Args:
            visitor (Any): The visitor to accept.

        Returns:
            Any: The result of the visitor's visit_block_stmt method.
        """
        return visitor.visit_block_stmt(self)


class IfStmtAST(StmtAST):
    """AST node representing an if statement.
    """

    def __init__(self, cond, then_block, else_block):
        """Initialize an IfStmtAST.

        Args:
            cond (ExprAST): The condition.
            then_block (StmtAST): The block to execute if the condition is
                true.
            else_block (StmtAST): The block to execute if the condition is
                false.
        """
        self.cond = cond
        self.then_block = then_block
        self.else_block = else_block

    def __repr__(self):
        return f"<{self.cond!r} {self.then_block!r} {self.else_block!r}>"

    def accept(self, visitor):
        """Accept a visitor.

        Args:
            visitor (Any): The visitor to accept.

        Returns:
            Any: The result of the visitor's visit_if_stmt method.
        """
        return visitor.visit_if_stmt(self)


class WhileStmtAST(StmtAST):
    """AST node representing a while statement.
    """

    def __init__(self, cond, body):
        """Initialize a WhileStmtAST.

        Args:
            cond (ExprAST): The condition.
            body (StmtAST): The body of the loop.
        """
        self.cond = cond
        self.body = body

    def __repr__(self):
        return f"<{self.cond!r} {self.body!r}>"

    def accept(self, visitor):
        """Accept a visitor.

        Args:
            visitor (Any): The visitor to accept.

        Returns:
            Any: The result of the visitor's visit_while_stmt method.
        """
        return visitor.visit_while_stmt(self)
