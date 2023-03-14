from typing import Iterable
from blast.interpreter import Interpreter
from argparse import ArgumentParser
import itertools


def main():
    argparser = ArgumentParser(description='Blast interpreter')

    mutex_args = argparser.add_mutually_exclusive_group(required=True)
    # either specify a file, execute an expression, or run in interactive mode (default)
    mutex_args.add_argument('file', nargs='?', type=str,
                            help='file to interpret')
    mutex_args.add_argument('-i', '--interactive',
                            action='store_true', help='run in interactive mode')
    mutex_args.add_argument('-e', '--expression',
                            type=str, help='execute an expression')

    args = argparser.parse_args()

    if args.interactive:
        repl()
    elif args.expression:
        expression(args.expression)
    else:
        read(args.file)


def repl():
    print('BLAST interpreter')
    print('Type in multiple lines of code, then press Ctrl+D (or Enter then Ctrl+Z then Enter on Windows) to execute.')
    print('Press Ctrl+C to exit.')

    interpreter = Interpreter()
    while True:
        lines = []  # list of lines of input
        # input until EOF
        while True:
            try:
                # if 1st line, prompt with '>>> ', otherwise '... '
                prompt = '>>> ' if not lines else '... '
                line = input(prompt)
                # if line is empty, break
                if not line:
                    break
                # append line to list of lines
                lines.append(line)
                # if single line and (when trimmed) ends with '.', break
                if len(lines) == 1 and line.strip().endswith('.'):
                    break
            except EOFError:
                break
            except KeyboardInterrupt:
                exit(0)

        try:
            results = interpreter.evaluate('\n'.join(lines))
            # check if empty list
            if results:
                # flatten list
                results = flatten(results)
                # print results
                print(*results, sep='\n')
        except Exception as e:
            print(e)


def expression(expr):
    interpreter = Interpreter()
    try:
        results = interpreter.evaluate(expr)
        # check if empty list
        if results:
            # flatten list
            results = flatten(results)
            # print results
            print(*results, sep='\n')
    except Exception as e:
        print(e)


def read(file):
    try:
        with open(file, 'r') as f:
            Interpreter().evaluate(f.read())
    except FileNotFoundError:
        print(f'File {file} not found.')
        exit(1)
    except Exception as e:
        print(e)
        exit(2)


def flatten(items):
    """Yield items from any nested iterable; see Reference."""
    for x in items:
        if isinstance(x, Iterable) and not isinstance(x, (str, bytes)):
            for sub_x in flatten(x):
                yield sub_x
        else:
            yield x


if __name__ == '__main__':
    main()
