from blast.scanner import Scanner

def main():
    source = '1 + 2 * 3 / 4'

    # create a new scanner instance
    scanner = Scanner(source)
    tokens = scanner.scan_tokens()

    # print the tokens
    print([repr(token) for token in tokens])

if __name__ == '__main__':
    main()