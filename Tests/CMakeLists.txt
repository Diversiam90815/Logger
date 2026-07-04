set(TEST_SOURCES
    test_Helper.cpp
    test_Formatter.cpp
    test_LoggerConfig.cpp
    test_LoggerWrapper.cpp
)

add_executable(LoggerTests ${TEST_SOURCES})

target_link_libraries(LoggerTests PRIVATE Logger spdlog::spdlog nlohmann_json::nlohmann_json)

AddTests(LoggerTests)
