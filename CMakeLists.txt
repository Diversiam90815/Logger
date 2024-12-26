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


set(SOURCE_DIR              ${PROJECT_SOURCE_DIR}/src/)
set(HEADER_DIR              ${PROJECT_SOURCE_DIR}/include/)
set(EXAMPLE_DIR             ${PROJECT_SOURCE_DIR}/Example/)


set(ALL_PROJECT_DIRS
    ${SOURCE_DIR}    
    ${HEADER_DIR}    
    ${EXAMPLE_DIR}    
)

set(Source_Files 
        ${SOURCE_DIR}/LoggerWrapper.cpp
        ${SOURCE_DIR}/PrintMacros.cpp
        ${SOURCE_DIR}/Formatter.cpp
)

set(Header_Files 
        ${HEADER_DIR}/LoggerWrapper.h  
        ${HEADER_DIR}/PrintMacros.h
        ${HEADER_DIR}/Formatter.h
)

set(Example_Files 
        ${EXAMPLE_DIR}/main.cpp
)


set(ALL_FILES
    ${Example_Files}    
    ${Source_Files}    
    ${Header_Files}    
)

source_group("Header"           FILES ${Source_Files})
source_group("Source"           FILES ${Header_Files})
source_group("Example"          FILES ${Example_Files})


# Create the target
add_executable(${PROJECT_NAME}    ${ALL_FILES})

target_sources(${PROJECT_NAME} PRIVATE  
        ${ALL_FILES} 
)


target_include_directories(${PROJECT_NAME} PUBLIC
        ${PROJECT_BINARY_DIR}
        ${CMAKE_BINARY_DIR}/include/
        ${CMAKE_BINARY_DIR}/libs/spdlog/include/
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
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
)

target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog)


set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
)