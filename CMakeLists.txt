cmake_minimum_required(VERSION 3.25)

project(Logger LANGUAGES CXX)

set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


## Setting the lib directory for project included packages
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)

## Include the CPM (Package Manager used by CMake)
include(cmake/cpm.cmake)


#----------------------------
# Juce API
#----------------------------

CPMAddPackage(
        NAME spdlog
        GITHUB_REPOSITORY gabime/spdlog
        VERSION 1.15.0
        SOURCE_DIR ${LIB_DIR}/spdlog
)


set(SOURCE_DIR   ${PROJECT_SOURCE_DIR}/src)


set(Logger_Files 
        ${SOURCE_DIR}/LoggerWrapper.h  ${SOURCE_DIR}/LoggerWrapper.cpp
)

set(ALL_FILES
    ${Logger_Files}    
)

source_group("LoggerWrapper"    FILES ${Logger_Files})


# Create the dll target
add_library(${PROJECT_NAME} SHARED
    ${ALL_FILES}
)

target_sources(${PROJECT_NAME} PRIVATE  
        ${ALL_FILES} 
)


target_include_directories(${PROJECT_NAME} PUBLIC
        ${PROJECT_BINARY_DIR}
        ${CMAKE_BINARY_DIR}/include/
        ${ALL_PROJECT_DIRS}
)


## Project compiler options
target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
                /Oi;
                /Gy
        >
        $<$<CONFIG:Debug>:
                /Od;
                /Z7
        >
        /sdl
        /MP
        /wd4146  # Suppress warning C4146
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
)


set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
)