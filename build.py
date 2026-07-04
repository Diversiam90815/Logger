import os
import sys

from scripts.paths import *
from scripts.build_runner import BuildRunner
from scripts.argument_parser import create_argument_parser


def main():
    parser = create_argument_parser()
    args = parser.parse_args()

    if not len(sys.argv) > 1:
        parser.print_help()
        exit(1)

    ''' Setting up environment '''
    os.chdir(ROOT_DIR)

    build_dir = get_build_dir(str(args.architecture))
    # Tests are built as part of the single top-level build tree (see Tests/CMakeLists.txt),
    # not a separately-configured build, so the test dir is the same as the main build dir.
    test_dir = build_dir
    runner = BuildRunner(root_dir=ROOT_DIR, build_dir=build_dir, project_name="Logger", version_var="LOGGER_VERSION")

    runner.update_environment()
    runner.update_app_version()

    print("==== Logger Configuration ====")
    print(f"Current Directory:          {ROOT_DIR}")
    print(f"Build Directory:            {build_dir}")
    print(f"Test Build Directory:       {test_dir}")
    print(f"Architecture:               {args.architecture}")

    if not args.prepare:
        print(f"Configuration:              {args.configuration}")

    print(f"Environment:                {runner.env}")
    print(f"Version:                    {runner.version}")
    print("=====================================")

    if args.prepare:
        runner.prepare_cmake_project(platform=args.platform, architecture=args.architecture)

    if args.runtest:
        runner.run_cpp_unit_tests(configuration=args.configuration, test_build_dir=test_dir, target="LoggerTests")

    if args.build:
        runner.create_build_generator(platform=args.platform, build=True, architecture=args.architecture, configuration=args.configuration)

    if not args.prepare:
        runner.run_cpp_unit_tests(configuration=args.configuration, test_build_dir=test_dir, target="LoggerTests")



if __name__ == "__main__":
    main()
