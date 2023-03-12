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
        print(Interpreter().evaluate(args.expression))
    else:
        with open(args.file, 'r') as f:
            Interpreter().evaluate(f.read())


def repl():
    interpreter = Interpreter()
    while True:
        try:
            results = interpreter.evaluate(input('>>> '))
            # check if empty list
            if results:
                # flatten list
                results = flatten(results)
                # print results
                print(*results, sep='\n')
        except KeyboardInterrupt:
            print('Bye!')
            break
        except Exception as e:
            print(e)


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
