from blast.token import Token
from blast.tokentype import TokenType

def main():
    token = Token(TokenType.NUMBER, 1)

    print(repr(token))

if __name__ == '__main__':
    main()