import unittest
from blast.scanner import Scanner
from blast.token import Token, TokenType


class TestScanner(unittest.TestCase):
    SOURCE = '1 +2* (3 -7) / 4.0'

    def test_scan_tokens(self):
        scanner = Scanner(self.SOURCE)
        tokens = scanner.scan_tokens()
        self.assertListEqual(tokens, [
            Token(TokenType.NUMBER, 1),
            Token(TokenType.PLUS, '+'),
            Token(TokenType.NUMBER, 2),
            Token(TokenType.MUL, '*'),
            Token(TokenType.LPAREN, '('),
            Token(TokenType.NUMBER, 3),
            Token(TokenType.MINUS, '-'),
            Token(TokenType.NUMBER, 7),
            Token(TokenType.RPAREN, ')'),
            Token(TokenType.DIV, '/'),
            Token(TokenType.NUMBER, 4.0),
        ])

    def test__is_at_end(self):
        scanner = Scanner(self.SOURCE)
        self.assertFalse(scanner._is_at_end())
        scanner._current = len(self.SOURCE)
        self.assertTrue(scanner._is_at_end())

    def test__advance(self):
        scanner = Scanner(self.SOURCE)
        self.assertEqual(scanner._current, 0)
        scanner._advance()
        self.assertEqual(scanner._current, 1)

    def test__skip_whitespace(self):
        scanner = Scanner(self.SOURCE)
        scanner._current = 1
        scanner._skip_whitespace()
        self.assertEqual(scanner._current, 2)

    def test__is_whitespace(self):
        scanner = Scanner(self.SOURCE)
        scanner._current = 0
        self.assertFalse(scanner._is_whitespace())
        scanner._current = 1
        self.assertTrue(scanner._is_whitespace())

    def test__scan_token(self):
        scanner = Scanner(self.SOURCE)
        scanner._scan_token()
        self.assertEqual(scanner._current, 1)
        scanner._scan_token()
        self.assertEqual(scanner._current, 3)
        scanner._scan_token()
        self.assertEqual(scanner._current, 4)
        scanner._scan_token()
        self.assertEqual(scanner._current, 5)
        scanner._scan_token()
        self.assertEqual(scanner._current, 7)
        scanner._scan_token()
        self.assertEqual(scanner._current, 8)
        scanner._scan_token()
        self.assertEqual(scanner._current, 10)
        scanner._scan_token()
        self.assertEqual(scanner._current, 11)
        scanner._scan_token()
        self.assertEqual(scanner._current, 12)
        scanner._scan_token()
        self.assertEqual(scanner._current, 14)
        scanner._scan_token()
        self.assertEqual(scanner._current, 18)


    def test__add_token(self):
        scanner = Scanner(self.SOURCE)
        scanner._add_token('1.0')
        self.assertListEqual(scanner._tokens, [Token(TokenType.NUMBER, 1.0)])