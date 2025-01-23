cmake_minimum_required(VERSION 3.25)

set(LOGGER_VERSION 1.1.34)

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
    ${HEADER_DIR}/LoggerRegistry.h
)

set(Example_Files 
    ${EXAMPLE_DIR}/main.cpp
)

# Create the library
add_library(${PROJECT_NAME} STATIC ${Source_Files} ${Header_Files})
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog)
target_include_directories(${PROJECT_NAME} PUBLIC ${HEADER_DIR})
