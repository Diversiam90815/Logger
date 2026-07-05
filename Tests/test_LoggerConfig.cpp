#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "LoggerConfig.h"


namespace
{

class LoggerConfigTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		configPath = std::filesystem::temp_directory_path() / "logger_config_test.json";
	}

	void TearDown() override
	{
		std::error_code ec;
		std::filesystem::remove(configPath, ec);
	}

	void writeConfig(const std::string &content)
	{
		std::ofstream file(configPath);
		file << content;
	}

	std::filesystem::path configPath;
};

} // namespace


TEST_F(LoggerConfigTest, ParsesValidJsonFile)
{
	writeConfig(R"({
		"sinks": [
			{ "type": "console", "level": "debug" }
		]
	})");

	LoggerConfig config(configPath.string());
	auto		 json = config.getConfig();

	ASSERT_TRUE(json.contains("sinks"));
	ASSERT_TRUE(json["sinks"].is_array());
	EXPECT_EQ(json["sinks"][0]["type"], "console");
	EXPECT_EQ(json["sinks"][0]["level"], "debug");
}

TEST_F(LoggerConfigTest, ThrowsWhenFileDoesNotExist)
{
	auto missingPath = std::filesystem::temp_directory_path() / "does_not_exist_logger_config.json";
	EXPECT_THROW(LoggerConfig(missingPath.string()), std::runtime_error);
}
