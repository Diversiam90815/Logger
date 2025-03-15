cmake_minimum_required(VERSION 3.25)

set(LOGGER_VERSION 1.1.37)

project(Logger LANGUAGES CXX VERSION ${LOGGER_VERSION})

set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Setting the lib directory for project included packages
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)

## Include the CPM (Package Manager used by CMake)
include(cmake/cpm.cmake)

#----------------------------
# SPDLOG
#----------------------------

CPMAddPackage(
    NAME spdlog
    GITHUB_REPOSITORY gabime/spdlog
    VERSION 1.15.0
    SOURCE_DIR ${LIB_DIR}/spdlog
)


#----------------------------
# NLOHMANN JSON
#----------------------------

CPMAddPackage(
    NAME nlohmann_json
    GITHUB_REPOSITORY nlohmann/json
    VERSION 3.11.2
    SOURCE_DIR ${LIB_DIR}/nlohmann_json
)


set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/)
set(HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/)
set(EXAMPLE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Example/)

set(Source_Files 
    ${SOURCE_DIR}/LoggerWrapper.cpp
    ${SOURCE_DIR}/Formatter.cpp
)

set(Header_Files 
    ${HEADER_DIR}/Logger.h  
    ${HEADER_DIR}/LoggerWrapper.h  
    ${HEADER_DIR}/PrintMacros.h
    ${HEADER_DIR}/Formatter.h
    ${HEADER_DIR}/Helper.h
    ${HEADER_DIR}/LoggerManager.h
    ${HEADER_DIR}/LoggerConfig.h
)

set(Example_Files 
    ${EXAMPLE_DIR}/main.cpp
)

# Create the library
add_library(${PROJECT_NAME} STATIC ${Source_Files} ${Header_Files})

target_link_libraries(${PROJECT_NAME} PUBLIC   spdlog::spdlog 
                                                nlohmann_json::nlohmann_json)

target_include_directories(${PROJECT_NAME} PUBLIC ${HEADER_DIR})


#----------------------------
#   Create Example Executable
#----------------------------

option(BUILD_EXAMPLE "Build the logger example executable" OFF)

if(BUILD_EXAMPLE)
    add_executable(LoggerExample ${Example_Files})
    target_link_libraries(LoggerExample PRIVATE Logger)
endif()


#----------------------------
# DEFINING JSON SETTING NAMES
#----------------------------

set(LOGGER_CONFIG_SINK "sinks" CACHE STRING "JSON key for logger sinks")
set(LOGGER_CONFIG_SINK_TYPE "type" CACHE STRING "JSON key for sink type")
set(LOGGER_CONFIG_LEVEL "level" CACHE STRING "JSON key for log level")
set(LOGGER_CONFIG_MAX_SKIP_DURATION "max_skip_duration" CACHE STRING "JSON key for max skip duration")
set(LOGGER_CONFIG_FILE_NAME "file_name" CACHE STRING "JSON key for file name")
set(LOGGER_CONFIG_MAX_FILE_SIZE "max_file_size" CACHE STRING "JSON key for max file size")
set(LOGGER_CONFIG_MAX_FILES "max_files" CACHE STRING "JSON key for max files")
set(LOGGER_CONFIG_ROTATE_ON_SESSION "rotate_on_session" CACHE STRING "JSON key for file rotation on session")
set(LOGGER_CONFIG_CHECK_FOR_DEBUGGER "check_for_debugger" CACHE STRING "JSON key for MSVC sink debugger check")
set(LOGGER_CONFIG_PATTERN "pattern" CACHE STRING "JSON key for log pattern")


configure_file(LoggerJSONConfigNames.h.in LoggerJSONConfigNames.h @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})