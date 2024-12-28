# Logger

A simple C++ logging setup that builds on [spdlog](https://github.com/gabime/spdlog) to provide easily configurable console and file outputs, along with convenient logging macros.

## Features
- **Console** and **File** output sinks.
- Configurable log levels, file sizes, and rotation behavior.
- Log macros (`LOG_INFO`, `LOG_ERROR`, etc.) with `fmt` formatting.
- Simple [CMake](https://cmake.org/) build and example usage.

## Quick Start

1. **Clone or download** this repository.
2. **Build** with [Python 3](https://www.python.org/) and CMake:
   ```bash
   python build.py --prepare   # Generates CMake build files
   python build.py --build     # Builds the project
   ```
   Alternatively, run CMake manually:
   ```bash
   cmake -B build
   cmake --build build --config Release
   ```
3. **Run** the generated `Logger.exe` (on Windows) or `Logger` (on other platforms) in the `build` directory.

## Usage Example

In your code:

```cpp
#include "LoggerWrapper.h"
#include "PrintMacros.h"

int main()
{
    // Setup file output
    logging::addFileOutput()
        .setFilename("TestLog.log")
        .setLevel(LogLevel::Info)
        .setMaxFileSize(20_MB)
        .setRotateOnSession(true);

    // Setup console output
    logging::addConsoleOutput().setLevel(LogLevel::Debug);

    LOG_INFO("Hello logger, the answer is: {}", 42);

    return 0;
}
```

## Notes
- Update `CMakeLists.txt` or `build.py` according to your needs.
- Requires **C++20** and **CMake ≥ 3.25**.
- By default, logs are written at *Info* level to the configured sinks.  
- The [spdlog](https://github.com/gabime/spdlog) library is included via CPM in the `libs` directory. 

Enjoy streamlined logging with SPDLog!