/*
  ==============================================================================

	Module			LoggerWrapper
	Description		A builder class to configure and create an SPDLog logger.

  ==============================================================================
*/

#include "LoggerWrapper.h"

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace
{
// Helper function to convert from LoggerBuilder::Level to spdlog::level::level_enum
spdlog::level::level_enum toSpdLogLevel(LoggerWrapper::Level level)
{
	switch (level)
	{
	case LoggerWrapper::Level::Trace: return spdlog::level::trace;
	case LoggerWrapper::Level::Debug: return spdlog::level::debug;
	case LoggerWrapper::Level::Info: return spdlog::level::info;
	case LoggerWrapper::Level::Warn: return spdlog::level::warn;
	case LoggerWrapper::Level::Error: return spdlog::level::err;
	case LoggerWrapper::Level::Critical: return spdlog::level::critical;
	default: return spdlog::level::info;
	}
}
} // anonymous namespace


LoggerWrapper &LoggerWrapper::setName(const std::string &name)
{
	mLoggerName = name;
	return *this;
}


LoggerWrapper &LoggerWrapper::setLogLevel(Level level)
{
	mLogLevel = level;
	return *this;
}


LoggerWrapper &LoggerWrapper::setLogPattern(const std::string &pattern)
{
	mPattern = pattern;
	return *this;
}


LoggerWrapper &LoggerWrapper::addRotatingLog(const std::string &filename, size_t maxFileSizeBytes, size_t maxFiles)
{
	auto sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(filename, // path to the log file
																	   maxFileSizeBytes, maxFiles
																	   // optional 4th param: rotate_on_open = false
	);
	mSinks.push_back(sink);
	return *this;
}


LoggerWrapper &LoggerWrapper::addConsoleOutput()
{
	auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	mSinks.push_back(sink);
	return *this;
}


std::shared_ptr<spdlog::logger> LoggerWrapper::build()
{
	if (mSinks.empty())
	{
		// If the user did not specify a sink, let's at least have a console sink
		auto defaultSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		mSinks.push_back(defaultSink);
	}

	// Create logger
	auto logger = std::make_shared<spdlog::logger>(mLoggerName, mSinks.begin(), mSinks.end());

	// Set pattern and level
	logger->set_pattern(mPattern);
	logger->set_level(toSpdLogLevel(mLogLevel));

	return logger;
}
