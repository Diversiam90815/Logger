/*
  ==============================================================================

	Module			LoggerWrapper
	Description		A builder class to configure and create an SPDLog logger.

  ==============================================================================
*/

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/dup_filter_sink.h>
#include <spdlog/logger.h>
#include <spdlog/fmt/fmt.h>

#include "Formatter.h"


class LoggerWrapper
{
public:
	LoggerWrapper() = default;

	enum class Level
	{
		Trace,
		Debug,
		Info,
		Warn,
		Error,
		Critical
	};


	std::shared_ptr<spdlog::logger> getOrCreateLogger(bool drop = false);


	void							registerSink(spdlog::sink_ptr sink, std::chrono::microseconds maxSkipDuration);
	void							dropAllAndCreateDefaultLogger();
	static std::string						sprintf(const char *format, ...);


	void							log(Level level, std::string_view func, std::string_view msg);


	LoggerWrapper				   &setName(const std::string &name);

	LoggerWrapper				   &setLogLevel(Level level);

	LoggerWrapper				   &setLogPattern(const std::string &pattern);

	LoggerWrapper				   &addRotatingLog(const std::string &filename, size_t maxFileSizeBytes, size_t maxFiles);

	LoggerWrapper				   &addConsoleOutput();

	std::shared_ptr<spdlog::logger> build();

private:
	std::string					  mLoggerName{"default_logger"};

	std::string					  mPattern{"%+"};

	Level						  mLogLevel{Level::Info};

	std::vector<spdlog::sink_ptr> mSinks; // We'll store the sinks here before creating the logger
};
