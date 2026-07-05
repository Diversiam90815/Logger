# Logger

[![Windows](https://github.com/Diversiam90815/Logger/actions/workflows/windows.yml/badge.svg)](https://github.com/Diversiam90815/Logger/actions/workflows/windows.yml)
[![Linux](https://github.com/Diversiam90815/Logger/actions/workflows/linux.yml/badge.svg)](https://github.com/Diversiam90815/Logger/actions/workflows/linux.yml)
[![macOS](https://github.com/Diversiam90815/Logger/actions/workflows/macos.yml/badge.svg)](https://github.com/Diversiam90815/Logger/actions/workflows/macos.yml)
[![Static Analysis](https://github.com/Diversiam90815/Logger/actions/workflows/static-analysis.yml/badge.svg)](https://github.com/Diversiam90815/Logger/actions/workflows/static-analysis.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

## Overview

A lightweight C++20 logging library built on top of [spdlog](https://github.com/gabime/spdlog), providing configurable console and file sinks, `std::format`-based logging macros, and JSON-driven configuration. spdlog and nlohmann json are internal implementation details only - consumers link a single `Logger` target and never reference either directly. The project ships with a GoogleTest suite, a unified Python-driven CMake build system, and a GitHub Actions pipeline that builds, tests, and statically analyzes every push.

## Features

- **Console** and **File** output sinks, plus an MSVC debug-output sink on Windows.
- Configurable log levels, file sizes, and rotation behavior.
- Log macros (`LOG_INFO`, `LOG_ERROR`, etc.) using `std::format` (C++20) - no external formatting library required.
- **JSON configuration initialization**: set up logger sinks via a JSON config file, with CMake-overridable key names.
- Dependencies are hidden: `spdlog`/`nlohmann_json` are `PRIVATE` to Logger and never exposed on a consumer's include path.
- Built-in unit test suite ([GoogleTest](https://github.com/google/googletest)) and CI (build, test, static analysis) via GitHub Actions.

## Technology Stack

| Category | Technology |
|---|---|
| Language | C++20 |
| Formatting | `std::format` (standard library) |
| Logging Backend | [spdlog](https://github.com/gabime/spdlog) 1.15.3 (private dependency) |
| Build System | CMake 3.25+, Python 3.x |
| Dependency Management | [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake) |
| Testing | GoogleTest 1.15.2 + CTest |
| Static Analysis | CppCheck |
| CI | GitHub Actions (build + test on Windows/Linux/macOS, cppcheck) |

## Project Structure

```
Logger/
├── cmake/                  # CMake modules (CPM, testing, cppcheck, package config, etc.)
├── scripts/                # Python build automation modules
├── src/                    # LoggerWrapper.cpp, Formatter.cpp, LoggerImpl.cpp
├── include/Logger/         # Public headers (Logger.h, LoggerWrapper.h, PrintMacros.h)
│   └── detail/             # Internal headers (Formatter.h, LoggerConfig.h, LoggerImpl.h)
├── Tests/                  # GoogleTest unit tests
├── Example/                # Example usage snippet
├── build/                  # Generated build artifacts
├── install/                # Installed headers, library, and CMake package
└── build.py                # Build entry point
```

## Prerequisites

- **C++ Compiler**: C++20 (MSVC 19.29+/VS2022, GCC 13+, or Clang 17+)
- **CMake**: Version 3.25 or higher
- **Python**: 3.x (for `build.py`)
- **Git**

### Optional Developer Tools

| Tool | CMake Option | Purpose | Install (Windows) |
|---|---|---|---|
| CppCheck | `LOGGER_ENABLE_CPPCHECK` (default `OFF`) | Static analysis on the `Logger` target | `winget install cppcheck` |

## Getting Started

### Cloning the Repository

```bash
git clone git@github.com:Diversiam90815/Logger.git
cd Logger
```

### Building

All builds go through `build.py`, which handles CMake configuration, compilation, and testing in a single command. Tests run automatically after any `--build` invocation.

```bash
# Release build (default) - configures, builds, installs, and runs tests
python build.py --build

# Debug build with Ninja
python build.py --build -c Debug -pl Ninja
```

### `build.py` Flag Reference

| Flag | Long form | Description |
|---|---|---|
| `-p` | `--prepare` | Run CMake configure only (no compile) |
| `-b` | `--build` | Configure, compile, install, and run tests |
| `-pl PLATFORM` | `--platform` | CMake generator: `Ninja` (default), `VS2022`, or `VS2026` |
| `-a ARCH` | `--architecture` | Target architecture: `x64` (default) or `ARM64` |
| `-c CONFIG` | `--configuration` | Build configuration: `Debug`, `Release` (default), or `RelWithDebInfo` |
| `-t` | `--runtest` | Run the test suite standalone, without rebuilding (e.g. `-t true`) |

Examples:

```bash
# Debug build targeting ARM64 with a Visual Studio 2022 solution
python build.py -b -c Debug -a ARM64 -pl VS2022

# Run tests only, against an existing build
python build.py -t true
```

### Build Outputs

| Artifact | Location |
|---|---|
| Logger static library | `build/<arch>/<Config>/Logger.lib` |
| Test executable | `build/<arch>/Tests/<Config>/LoggerTests` |
| Installed headers, library, and CMake package | `install/` |

### Running Tests

```bash
python build.py -b -t true                                  # build then run tests
ctest --test-dir build/x64 -C Release --output-on-failure   # or run CTest directly
```

Tests are discovered automatically via `gtest_discover_tests()`.

## Add Logger to Your Project

There are two supported ways to consume Logger, depending on whether you want to build it from source alongside your project or link against a pre-built install.

### Option 1: As a subdirectory (source build)

Add it as a subdirectory (e.g. a git submodule under `submodules/Logger`) in your top-level `CMakeLists.txt`:

```cmake
add_subdirectory(submodules/Logger)
```

Then link it to your target:

```cmake
target_link_libraries(MyApp PRIVATE Logger)
```

Logger's public headers (`Logger.h`, `LoggerWrapper.h`, `PrintMacros.h`) are exposed via a `PUBLIC` include directory, so no extra `include_directories()` calls are needed. CPM fetches spdlog/nlohmann_json automatically the first time you configure.

### Option 2: Against an installed package (`find_package`)

If you'd rather build Logger once and install it (`python build.py --build`, or `cmake --install build --prefix /path/to/install`), it produces a real, `find_package()`-able CMake package - spdlog is installed alongside it (needed to satisfy the final link, since it's a private/compiled dependency), but you never reference it directly:

```cmake
find_package(Logger CONFIG REQUIRED PATHS /path/to/install)
target_link_libraries(MyApp PRIVATE Logger::Logger)
```

## In your code

### Programmatic Initialization

You can manually add sinks using the provided builder methods:

```cpp
#include "Logger.h"

int main()
{
    // Setup file output with custom options
    logging::addFileOutput()
        .setFilename("TestLog.log")
        .setLevel(LogLevel::Info)
        .setMaxFileSize(20_MB)
        .setRotateOnSession(true);

    // Setup console output with custom level
    logging::addConsoleOutput().setLevel(LogLevel::Debug);

    LOG_INFO("Hello logger, the answer is: {}", 42);

    return 0;
}
```

Which produces a log line like:

```
Timestamp                 ThreadID Level    Source Module        Function                                      Message
2026-07-04 23:06:04.729      25036 info     main                 main                                          Hello logger, the answer is: 42
```

### JSON Configuration Initialization

Alternatively, you can initialize the logger via a JSON configuration file. Create a JSON file (e.g. `logger_config.json`) with the following structure:

```json
{
    "sinks": [
        {
            "type": "console",
            "level": "debug",
            "max_skip_duration": 0,
            "pattern": "[%Y-%m-%d %H:%M:%S.%e] [%l] %v"
        },
        {
            "type": "file",
            "level": "info",
            "max_skip_duration": 0,
            "file_name": "logs/app.log",
            "max_file_size": "10_MB",
            "max_files": 3,
            "rotate_on_session": true
        }
    ]
}
```

Then in your code initialize the logger using:

```cpp
#include "Logger.h"

int main()
{
    // Initialize logger with JSON configuration.
    // Missing settings will be replaced with default values.
    logging::initializeLogger("logger_config.json");

    LOG_INFO("Logger initialized using JSON configuration!");

    return 0;
}

```

This approach leverages [nlohmann json](https://github.com/nlohmann/json) (integrated via CPM, internal only) to parse the configuration file and set up the sinks accordingly. If a key is missing, a sensible default will be used.

### Overriding JSON Configuration Keys

This project defines the JSON key names (used in the configuration file) as CMake Cache variables. This means that when you include Logger as a subdirectory in your project, you can easily override these defaults without modifying the Logger source code.

### How to override

Before adding the Logger subdirectory in your parent project's CMakeLists.txt, set the desired values for the JSON keys. For example:

```cmake
# Override default JSON key names
set(LOGGER_CONFIG_SINK "custom_sinks" CACHE STRING "JSON key for logger sinks")
set(LOGGER_CONFIG_LEVEL "custom_level" CACHE STRING "JSON key for log level")
# ...set other keys as needed

add_subdirectory(path/to/Logger)
```

Alternatively, you can override these values on the command line when configuring your project:

```bash
cmake -DLOGGER_CONFIG_SINK="custom_sinks" -DLOGGER_CONFIG_LEVEL="custom_level" <path-to-your-project>
```

## Default values

If not otherwise specified, the logger will provide default values:

### Console Sink Defaults
- **Log Level** : `info`
- **Max Skip Duration** : `0` (microseconds)
- **Pattern** : `[%Y-%m-%d %H:%M:%S.%e] [%l] %v`

### File Sink Defaults
- **Log Level** : `info`
- **Max Skip Duration** : `0` (microseconds)
- **File Name** : `default.log`
- **Max File Size** : `10 MB`
- **Max Files** : `3`
- **Rotate on session** : `false`

### MSVC Sink Defaults (Windows Only)
- **Log Level** : `info`
- **Max Skip Duration** : `0` (microseconds)
- **Check For Debugger** : `false`

## Build System Architecture

The build system is split into two layers: a **Python layer** that provides a consistent developer interface, and a **CMake layer** that performs the actual configuration and compilation.

### Python Layer (`scripts/`)

Rather than a single monolithic script, the build automation is broken into focused modules:

| Module | Responsibility |
|---|---|
| `build_runner.py` | Invokes CMake configure, build, install, and test commands |
| `argument_parser.py` | CLI argument parsing and validation |
| `enums.py` | Typed enums for architecture, configuration, and platform |
| `paths.py` | Centralised path constants (root, build, install directories) |
| `versioning.py` | Reads `git rev-list` commit count and updates `LOGGER_VERSION` before each configure |
| `env_config.py` | Detects environment (Development / Staging / Production) from the active git branch |
| `utils.py` | Subprocess execution helpers and a `working_directory` context manager |

### CMake Layer (`cmake/`)

| Module | Responsibility |
|---|---|
| `cpm.cmake` | CPM package manager: downloads all dependencies at configure time |
| `Testing.cmake` | Integrates GoogleTest and CTest (also wires up an opt-in Valgrind memcheck target on Linux) |
| `CppCheck.cmake` | Attaches cppcheck as a `CXX_CPPCHECK` property on a target, gated by `LOGGER_ENABLE_CPPCHECK` |

### Version Management

The project version follows `MAJOR.MINOR.PATCH`. The patch number is derived automatically from the total git commit count (`git rev-list HEAD --count`) and written back to `CMakeLists.txt` before each configure step via `build.py`.

## External Dependencies

All dependencies are managed by CPM.cmake and downloaded automatically at configure time; no manual installation is required beyond the tools listed in [Prerequisites](#prerequisites).

| Dependency | Version | Purpose |
|---|---|---|
| spdlog | 1.15.3 | Logging backend (private) |
| nlohmann json | 3.12.0 | JSON config parsing (private, internal only) |
| GoogleTest | 1.15.2 | Unit testing |

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
