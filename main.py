from blast.scanner import Scanner
from blast.parser import Parser
from blast.ast import *


def main():
    source = '1 + 2 * 3 / 4.0'

    # create a new scanner instance
    scanner = Scanner(source)
    tokens = scanner.scan_tokens()

    # print the tokens
    print([repr(token) for token in tokens])

    parser = Parser(tokens)
    ast = parser.parse()

    print(ast)


if __name__ == '__main__':
    main()
