import unittest
from blast.parser import Parser
from blast.token import Token, TokenType
from blast.ast import *


class TestParser(unittest.TestCase):
    def test_parse(self):
        tokens = [
            Token(TokenType.NUMBER, 1.0),
            Token(TokenType.PLUS, '+'),
            Token(TokenType.NUMBER, 2.0),
            Token(TokenType.MUL, '*'),
            Token(TokenType.NUMBER, 3.0),
            Token(TokenType.DIV, '/'),
            Token(TokenType.NUMBER, 4.0)
        ]
        parser = Parser(tokens=tokens)
        ast = parser.parse()
        self.assertIsInstance(ast, BinaryExprAST)
        self.assertIsInstance(ast.lhs, NumberExprAST)
        self.assertIsInstance(ast.rhs, BinaryExprAST)
        self.assertIsInstance(ast.rhs.lhs, BinaryExprAST)
        self.assertIsInstance(ast.rhs.rhs, NumberExprAST)
        self.assertIsInstance(ast.rhs.lhs.lhs, NumberExprAST)
        self.assertIsInstance(ast.rhs.lhs.rhs, NumberExprAST)

    def test__expression(self):
        tokens = [
            Token(TokenType.NUMBER, 1.0),
            Token(TokenType.PLUS, '+'),
            Token(TokenType.NUMBER, 2.0),
            Token(TokenType.MUL, '*'),
            Token(TokenType.NUMBER, 3.0),
            Token(TokenType.DIV, '/'),
            Token(TokenType.NUMBER, 4.0)
        ]
        parser = Parser(tokens=tokens)
        ast = parser._expression()
        self.assertIsInstance(ast, BinaryExprAST)
        self.assertIsInstance(ast.lhs, NumberExprAST)
        self.assertIsInstance(ast.rhs, BinaryExprAST)
        self.assertIsInstance(ast.rhs.lhs, BinaryExprAST)
        self.assertIsInstance(ast.rhs.rhs, NumberExprAST)
        self.assertIsInstance(ast.rhs.lhs.lhs, NumberExprAST)
        self.assertIsInstance(ast.rhs.lhs.rhs, NumberExprAST)

    def test__addition(self):
        tokens = [
            Token(TokenType.NUMBER, 1.0),
            Token(TokenType.PLUS, '+'),
            Token(TokenType.NUMBER, 2.0),
            Token(TokenType.MUL, '*'),
            Token(TokenType.NUMBER, 3.0),
            Token(TokenType.DIV, '/'),
            Token(TokenType.NUMBER, 4.0)
        ]
        parser = Parser(tokens=tokens)
        ast = parser._addition()
        self.assertIsInstance(ast, BinaryExprAST)
        self.assertIsInstance(ast.lhs, NumberExprAST)
        self.assertIsInstance(ast.rhs, BinaryExprAST)
        self.assertIsInstance(ast.rhs.lhs, BinaryExprAST)
        self.assertIsInstance(ast.rhs.rhs, NumberExprAST)
        self.assertIsInstance(ast.rhs.lhs.lhs, NumberExprAST)
        self.assertIsInstance(ast.rhs.lhs.rhs, NumberExprAST)

    def test__multiplication(self):
        tokens = [
            Token(TokenType.NUMBER, 1.0),
            Token(TokenType.PLUS, '+'),
            Token(TokenType.NUMBER, 2.0),
        ]
        parser = Parser(tokens=tokens)
        ast = parser._multiplication()
        self.assertIsInstance(ast, NumberExprAST) # only one node, because it only recognizes the multiplication/division operations

    def test__unary(self):
        tokens = [
            Token(TokenType.MINUS, '-'),
            Token(TokenType.NUMBER, 1.0),
        ]
        parser = Parser(tokens=tokens)
        ast = parser._unary()
        self.assertIsInstance(ast, UnaryExprAST)
        self.assertEqual(ast.op.type, TokenType.MINUS)

    def test__primary(self):
        tokens = [
            Token(TokenType.NUMBER, 1.0),
        ]
        parser = Parser(tokens=tokens)
        ast = parser._primary()
        self.assertIsInstance(ast, NumberExprAST)

    def test__check(self):
        tokens = [
            Token(TokenType.NUMBER, 1.0),
            Token(TokenType.PLUS, '+'),
            Token(TokenType.NUMBER, 2.0),
        ]
        parser = Parser(tokens=tokens)
        result = parser._check([TokenType.NUMBER])
        self.assertTrue(result)
        parser._advance()
        result = parser._check([TokenType.PLUS])
        self.assertTrue(result)

    def test__advance(self):
        tokens = [
            Token(TokenType.NUMBER, 1.0),
            Token(TokenType.PLUS, '+'),
            Token(TokenType.NUMBER, 2.0),
        ]
        parser = Parser(tokens=tokens)
        parser._advance()
        self.assertEqual(parser._current, 1)
        self.assertEqual(parser._tokens[parser._current].type, TokenType.PLUS)

    def test__consume(self):
        tokens = [
            Token(TokenType.NUMBER, 1.0),
            Token(TokenType.PLUS, '+'),
            Token(TokenType.NUMBER, 2.0),
        ]
        parser = Parser(tokens=tokens)
        parser._consume([TokenType.NUMBER])
        self.assertEqual(parser._current, 1)
        self.assertEqual(parser._tokens[parser._current].type, TokenType.PLUS)

    def test__is_at_end(self):
        tokens = [
            Token(TokenType.NUMBER, 1.0),
            Token(TokenType.PLUS, '+'),
            Token(TokenType.NUMBER, 2.0),
        ]
        parser = Parser(tokens=tokens)
        self.assertFalse(parser._is_at_end())
        parser._advance()
        parser._advance()
        parser._advance()
        self.assertTrue(parser._is_at_end())

    def test__advance(self):
        tokens = [
            Token(TokenType.NUMBER, 1.0),
            Token(TokenType.PLUS, '+'),
            Token(TokenType.NUMBER, 2.0),
        ]
        parser = Parser(tokens=tokens)
        parser._advance()
        self.assertEqual(parser._current, 1)
        self.assertEqual(parser._tokens[parser._current].type, TokenType.PLUS)
