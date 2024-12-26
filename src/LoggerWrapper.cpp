/*
  ==============================================================================

	Module			LoggerWrapper
	Description		A builder class to configure and create an SPDLog logger.

  ==============================================================================
*/

#include "LoggerWrapper.h"

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <stdarg.h>

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


std::shared_ptr<spdlog::logger> LoggerWrapper::getOrCreateLogger(bool drop)
{
	if (drop)
	{
		spdlog::drop_all();
	}

	std::shared_ptr<spdlog::logger> logger;

	if (spdlog::default_logger() != nullptr)
	{
		logger = spdlog::default_logger();
	}
	else
	{
		static std::mutex			create_mutex;
		std::lock_guard<std::mutex> lock(create_mutex);

		auto					   &loggerName = mLoggerName;
		logger								   = std::make_shared<spdlog::logger>(loggerName);
		logger->set_level(spdlog::level::trace);
		logger->flush_on(spdlog::level::trace);
	}

	return logger;
}


void LoggerWrapper::registerSink(spdlog::sink_ptr sink, std::chrono::microseconds maxSkipDuration)
{
	sink->set_formatter(std::make_unique<Formatter>());

	static std::mutex			sink_mutex;
	std::lock_guard<std::mutex> lock(sink_mutex);

	bool						needReset = mSinks.empty();
	if (needReset)
	{
		// dropAllAndCreateDefaultLogger();
	}

	if (maxSkipDuration > std::chrono::seconds(0))
	{
		auto dupFilter = std::make_shared<spdlog::sinks::dup_filter_sink_mt>(maxSkipDuration);
		dupFilter->add_sink(sink);
		mSinks.push_back(dupFilter);
	}
	else
	{
		mSinks.push_back(sink);
	}

	// set sinks for all loggers
	spdlog::apply_all([this](std::shared_ptr<spdlog::logger> l) { l->sinks().assign(mSinks.begin(), mSinks.end()); });
}


void LoggerWrapper::dropAllAndCreateDefaultLogger()
{
	getOrCreateLogger(true);
}


void LoggerWrapper::log(Level level, std::string_view func, std::string_view msg)
{
	auto			   logger	= getOrCreateLogger();
	auto			   spdLevel = toSpdLogLevel(level);

	spdlog::source_loc loc{" ", 1, func.data()};

	logger->log(loc, spdLevel, msg);
}


std::string LoggerWrapper::sprintf(const char *format, ...)
{
	va_list args;
	va_start(args, format);

	va_list tmp;
	va_copy(tmp, args);

	std::string result;

	int			size = vsnprintf(nullptr, 0, format, args);
	va_end(args);

	va_start(tmp, format);

	if (size > 0)
	{
		size_t sz = static_cast<size_t>(size + 1);
		result.resize(sz + 1);
		vsnprintf(result.data(), sz, format, tmp);
	}

	va_end(tmp);
	return result;
}


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
