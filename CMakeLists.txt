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
set(WRAPPER_DIR   ${SOURCE_DIR}/LoggerWrapper)
set(EXAMPLE_DIR   ${SOURCE_DIR}/Example)


set(ALL_PROJECT_DIRS
    ${WRAPPER_DIR}    
    ${EXAMPLE_DIR}    
)

set(Logger_Files 
        ${WRAPPER_DIR}/LoggerWrapper.h  ${WRAPPER_DIR}/LoggerWrapper.cpp
)

set(Example_Files 
        ${EXAMPLE_DIR}/main.cpp
)

set(ALL_FILES
    ${Example_Files}    
    ${Logger_Files}    
)

source_group("LoggerWrapper"    FILES ${Logger_Files})
source_group("Example"          FILES ${Example_Files})


# Create the target
add_executable(${PROJECT_NAME}
    ${ALL_FILES}
)

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