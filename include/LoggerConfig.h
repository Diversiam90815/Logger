/*
==============================================================================
	Module			LoggerConfig
	Description		Initializing the logger with a JSON config file
==============================================================================
*/

#pragma once

#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>


using json = nlohmann::json;

#define LOGGER_CONFIG_SINK				 "@LOGGER_CONFIG_SINK@"
#define LOGGER_CONFIG_SINK_TYPE			 "@LOGGER_CONFIG_SINK_TYPE@"
#define LOGGER_CONFIG_LEVEL				 "@LOGGER_CONFIG_LEVEL@"
#define LOGGER_CONFIG_MAX_SKIP_DURATION	 "@LOGGER_CONFIG_MAX_SKIP_DURATION@"
#define LOGGER_CONFIG_FILE_NAME			 "@LOGGER_CONFIG_FILE_NAME@"
#define LOGGER_CONFIG_MAX_FILE_SIZE		 "@LOGGER_CONFIG_MAX_FILE_SIZE@"
#define LOGGER_CONFIG_MAX_FILES			 "@LOGGER_CONFIG_MAX_FILES@"
#define LOGGER_CONFIG_ROTATE_ON_SESSION	 "@LOGGER_CONFIG_ROTATE_ON_SESSION@"
#define LOGGER_CONFIG_CHECK_FOR_DEBUGGER "@LOGGER_CONFIG_CHECK_FOR_DEBUGGER@"
#define LOGGER_CONFIG_PATTERN			 "@LOGGER_CONFIG_PATTERN@"


class LoggerConfig
{
public:
	LoggerConfig(const std::string &configFilePath)
	{
		std::ifstream file(configFilePath);
		if (!file.is_open())
		{
			throw std::runtime_error("Could not open config file!");
		}

		file >> config;
	}

	json getConfig() const { return config; }

private:
	json config;
};