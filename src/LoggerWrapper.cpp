/*
  ==============================================================================

	Module			LoggerWrapper
	Description		A builder class to configure and create an SPDLog logger.

  ==============================================================================
*/

#include "LoggerWrapper.h"

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <iostream>
#include <stdarg.h>

namespace
{
// Helper function to convert from LoggerBuilder::Level to spdlog::level::level_enum
spdlog::level::level_enum toSpdLogLevel(LogLevel level)
{
	switch (level)
	{
	case LogLevel::Trace: return spdlog::level::trace;
	case LogLevel::Debug: return spdlog::level::debug;
	case LogLevel::Info: return spdlog::level::info;
	case LogLevel::Warn: return spdlog::level::warn;
	case LogLevel::Error: return spdlog::level::err;
	case LogLevel::Critical: return spdlog::level::critical;
	default: return spdlog::level::info;
	}
}
} // anonymous namespace


namespace logging
{

void addConsoleOutput(LogLevel level, std::chrono::microseconds maxSkipDuration)
{
	try
	{
		auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		sink->set_level(toSpdLogLevel(level));

		registerSink(sink, maxSkipDuration);
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error creating console output sink " << e.what() << std::endl;
	}
}


void addFileOutput(LogLevel level, std::chrono::microseconds maxSkipDuration, std::string fileName, size_t maxFileSize, size_t maxFiles, bool rotateOnSession)
{
	try
	{
		auto sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(std::move(fileName), maxFileSize, maxFiles, rotateOnSession);
		sink->set_level(toSpdLogLevel(level));

		registerSink(sink, maxSkipDuration);
	}

	catch (const std::exception &e)
	{
		std::cerr << "Error creating rotating file sink " << e.what() << std::endl;
	}
}


std::shared_ptr<spdlog::logger> getOrCreateLogger(bool drop)
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

		auto					   &loggerName = "DefaultLogger";
		logger								   = std::make_shared<spdlog::logger>(loggerName);
		logger->set_level(spdlog::level::trace);
		logger->flush_on(spdlog::level::trace);

		spdlog::register_logger(logger);
		spdlog::set_default_logger(logger);
	}

	return logger;
}


void registerSink(spdlog::sink_ptr sink, std::chrono::microseconds maxSkipDuration)
{
	sink->set_formatter(std::make_unique<Formatter>());

	static std::mutex			sink_mutex;
	std::lock_guard<std::mutex> lock(sink_mutex);

	auto					   &sinks	  = LoggerRegistry::sInstance().sinks();
	bool						needReset = sinks.empty();
	if (needReset)
	{
		 //dropAllAndCreateDefaultLogger();		// Seems to cause DefaultLogger (Logger Name) to appear as a file/modulename
	}

	if (maxSkipDuration > std::chrono::seconds(0))
	{
		auto dupFilter = std::make_shared<spdlog::sinks::dup_filter_sink_mt>(maxSkipDuration);
		dupFilter->add_sink(sink);
		sinks.push_back(dupFilter);
	}
	else
	{
		sinks.push_back(sink);
	}

	// set sinks for all loggers
	spdlog::apply_all([sinks](std::shared_ptr<spdlog::logger> l) { l->sinks().assign(sinks.begin(), sinks.end()); });
}


void dropAllAndCreateDefaultLogger()
{
	getOrCreateLogger(true);
}


void log(LogLevel level, const spdlog::source_loc &loc, std::string_view msg)
{
	auto logger	  = getOrCreateLogger();
	auto spdLevel = toSpdLogLevel(level);

	logger->log(loc, spdLevel, msg);
}



ConsoleOptions addConsoleOutput()
{
	return {};
}


FileOptions addFileOutput()
{
	return {};
}


} // namespace logging
