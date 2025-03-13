/*
==============================================================================
	Module			LoggerWrapper
	Description		A builder class to configure and create an SPDLog logger.
==============================================================================
*/

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/msvc_sink.h>
#include <iostream>

#include "LoggerWrapper.h"


namespace logging
{

void addConsoleOutput(LogLevel level, std::chrono::microseconds maxSkipDuration, std::string pattern)
{
	try
	{
		auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		sink->set_level(toSpdLogLevel(level));
		sink->set_pattern(pattern);

		registerSink(sink, maxSkipDuration);
	}
	catch (const std::exception &e)
	{
		auto logger = getOrCreateLogger();
		logger->error("Error creating console output sink : {}", e.what());
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
		auto logger = getOrCreateLogger();
		logger->error("Error creating rotating file sink : {}", e.what());
	}
}


void addMSVCOutput(LogLevel level, bool checkForDebuggerPresent, std::chrono::microseconds maxSkipDuration)
{
#ifdef _WIN32
	auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>(checkForDebuggerPresent);
	sink->set_level(toSpdLogLevel(level));

	registerSink(sink, maxSkipDuration);
#endif // _WIN32
}

void initializeLogger(const std::string &configFilePath)
{
	LoggerConfig config(configFilePath);
	auto		 jsonConfig = config.getConfig();

	for (auto &sinkConfig : jsonConfig["sinks"])
	{
		std::string type  = sinkConfig["type"];
		LogLevel	level = toLogLevel(sinkConfig["level"]);

		if (type == "console")
		{
			addConsoleOutput(level, std::chrono::milliseconds(sinkConfig["maxSkipDuration"]), sinkConfig["pattern"]);
		}
		else if (type == "file")
		{
			addFileOutput(level, std::chrono::microseconds(sinkConfig["maxSkipDuration"]), sinkConfig["fileName"], sinkConfig["maxFileSize"], sinkConfig["maxFiles"],
						  sinkConfig["rotateOnSession"]);
		}
		else if (type == "msvc")
		{
			addMSVCOutput(level, sinkConfig["checkForDebuggerPresent"], std::chrono::microseconds(sinkConfig["maxSkipDuration"]));
		}
	}
}


std::shared_ptr<spdlog::logger> getOrCreateLogger(bool drop)
{
	static std::once_flag				   flag;
	static std::shared_ptr<spdlog::logger> logger;

	if (drop)
	{
		spdlog::drop_all();
	}

	std::call_once(flag,
				   []()
				   {
					   auto loggerName = "DefaultLogger";
					   logger		   = std::make_shared<spdlog::logger>(loggerName);
					   logger->set_level(spdlog::level::trace);
					   logger->flush_on(spdlog::level::trace);
					   spdlog::register_logger(logger);
					   spdlog::set_default_logger(logger);
				   });

	return logger;
}


void registerSink(spdlog::sink_ptr sink, std::chrono::microseconds maxSkipDuration, std::unique_ptr<spdlog::formatter> formatter)
{
	if (formatter)
	{
		sink->set_formatter(std::move(formatter));
	}

	static std::mutex			sink_mutex;
	std::lock_guard<std::mutex> lock(sink_mutex);

	auto					   &sinks = LoggerManager::GetInstance()->sinks();

	if (maxSkipDuration > std::chrono::microseconds(0))
	{
		auto dupFilter = std::make_shared<spdlog::sinks::dup_filter_sink_mt>(maxSkipDuration);
		dupFilter->set_level(sink->level());
		dupFilter->add_sink(sink);
		sinks.push_back(dupFilter);
	}
	else
	{
		sinks.push_back(sink);
	}

	// set sinks for all loggers
	spdlog::apply_all(
		[sinks](std::shared_ptr<spdlog::logger> l)
		{
			l->sinks().assign(sinks.begin(), sinks.end());
			l->set_level(spdlog::level::debug);
		});
}


void log(LogLevel level, const spdlog::source_loc &loc, std::string_view msg)
{
	auto logger	  = getOrCreateLogger();
	auto spdLevel = toSpdLogLevel(level);

	logger->log(loc, spdLevel, msg);
}


void setLoggerName(std::string &name)
{
	LoggerManager::GetInstance()->setLoggerName(name);
}


std::string getLoggerName()
{
	return LoggerManager::GetInstance()->getLoggerName();
}



// Options:

// File Options:

FileOptions &FileOptions::setFilename(std::string &filename)
{
	this->filename = filename;
	return *this;
}

FileOptions &FileOptions::setMaxFileSize(size_t maxFileSize)
{
	this->maxFileSize = maxFileSize;
	return *this;
}

FileOptions &FileOptions::setMaxFiles(size_t maxFiles)
{
	this->maxFiles = maxFiles;
	return *this;
}

FileOptions &FileOptions::setRotateOnSession(bool rotateOnSession)
{
	this->rotateOnSession = rotateOnSession;
	return *this;
}

// MSVC Options:

MSVCOptions &MSVCOptions::checkForPresentDebugger(bool check)
{
	this->checkForDebugger = check;
	return *this;
}



ConsoleOptions addConsoleOutput()
{
	return {};
}

FileOptions addFileOutput()
{
	return {};
}

MSVCOptions addMSVCOutput()
{
	return {};
}

} // namespace logging
