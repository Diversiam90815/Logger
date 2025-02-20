# Logger

A simple C++ logging setup that builds on [spdlog](https://github.com/gabime/spdlog) to provide easily configurable console and file outputs, along with convenient logging macros.

## Features
- **Console** and **File** output sinks.
- Configurable log levels, file sizes, and rotation behavior.
- Log macros (`LOG_INFO`, `LOG_ERROR`, etc.) with `fmt` formatting.
- Simple [CMake](https://cmake.org/) build and example usage.


## Usage Example

### In your CMakeLists.txt:
#### Add Logger to Your Project
You can include this Logger project as a subdirectory in your own CMake project. For example, if you clone or include it as a git submodule under submodules/Logger, you can do:

#### In your top-level CMakeLists.txt
```cmake
add_subdirectory(submodules/Logger)
```

#### Link Logger to Your Executable/Library
Next, link the Logger library to your target. For instance:

```cmake
target_link_libraries(MyApp PRIVATE Logger)
```

Because the Logger uses a PUBLIC include for its headers, you do not need extra include_directories for the Logger — they are automatically made available to your target.

### In your code:

```cpp
#include "Logger.h"

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
