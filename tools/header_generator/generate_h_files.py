import os
import argparse
from generate_target_id import generate_target_id_file

def main(args):
    if not os.path.isdir(args.include_path):
        print(f"[ERROR] No such directory: {args.include_path}")
        exit()

    generate_target_id_file(args.include_path, args.use_cache)

def parse_arguments():
    parser = argparse.ArgumentParser(
        prog=os.path.basename(__file__),
        description="Automatically generate project header files",
        epilog="Generated files are: fopd_target_id.h"
    )

    parser.add_argument(
        '-i',
        '--include-path',
        help="Path to the project's include directory",
        required=True
    )

    parser.add_argument(
        '-c',
        '--use-cache',
        help="Use cache when available instead of scraping the web",
        action='store_true',
        required=False
    )

    return parser.parse_args()


if __name__ == '__main__':
    args = parse_arguments()
    main(args)
