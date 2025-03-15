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
#include "LoggerImpl.h"


using namespace filesize;

namespace logging
{
void addConsoleOutput(LogLevel level, std::chrono::microseconds maxSkipDuration, const std::string &pattern)
{
	LoggerImpl::GetInstance().addConsoleOutput(level, maxSkipDuration, pattern);
}

void addFileOutput(LogLevel level, std::chrono::microseconds maxSkipDuration, const std::string &fileName, size_t maxFileSize, size_t maxFiles, bool rotateOnSession)
{
	LoggerImpl::GetInstance().addFileOutput(level, maxSkipDuration, fileName, maxFileSize, maxFiles, rotateOnSession);
}

void addMSVCOutput(LogLevel level, bool checkForDebuggerPresent, std::chrono::microseconds maxSkipDuration)
{
	LoggerImpl::GetInstance().addMSVCOutput(level, checkForDebuggerPresent, maxSkipDuration);
}

void initializeLogger(const std::string &configFilePath)
{
	LoggerImpl::GetInstance().initializeLogger(configFilePath);
}

void log(LogLevel level, const std::string &file, int line, const std::string &function, const std::string &msg)
{
	LoggerImpl::GetInstance().log(level, file, line, function, msg);
}


//unsigned long long getFileSize(const json &j, const std::string &key, unsigned long long defaultValue)
//{
//	if (j.contains(key))
//	{
//		if (j[key].is_number_unsigned())
//		{
//			return j[key].get<unsigned long long>();
//		}
//		else if (j[key].is_string())
//		{
//			return parseFileSize(j[key].get<std::string>());
//		}
//		else
//		{
//			throw std::runtime_error("Invalid type for file size in config for key: " + key);
//		}
//	}
//	return defaultValue;
//}
//
//
//void initializeLogger(const std::string &configFilePath)
//{
//	LoggerConfig config(configFilePath);
//	auto		 jsonConfig = config.getConfig();
//
//	if (!jsonConfig.contains(LOGGER_CONFIG_SINK))
//	{
//		addConsoleOutput(LogLevel::Info, std::chrono::microseconds(0), "[%Y-%m-%d %H:%M:%S.%e] [%l] %v"); // Adding basic Console output for logger by default
//		return;
//	}
//
//
//	for (auto &sinkConfig : jsonConfig[LOGGER_CONFIG_SINK])
//	{
//		// Use default type "console" and level "info" if missing.
//		std::string type	 = sinkConfig.value(LOGGER_CONFIG_SINK_TYPE, "console");
//		std::string levelStr = sinkConfig.value(LOGGER_CONFIG_LEVEL, "info");
//		LogLevel	level	 = toLogLevel(levelStr);
//
//		if (type == "console")
//		{
//			auto		maxSkipDuration = std::chrono::milliseconds(sinkConfig.value(LOGGER_CONFIG_MAX_SKIP_DURATION, 0));
//			std::string pattern			= sinkConfig.value(LOGGER_CONFIG_PATTERN, "[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
//
//			addConsoleOutput(level, maxSkipDuration, pattern);
//		}
//		else if (type == "file")
//		{
//			auto		maxSkipDuration = std::chrono::microseconds(sinkConfig.value(LOGGER_CONFIG_MAX_SKIP_DURATION, 0));
//			std::string fileName		= sinkConfig.value(LOGGER_CONFIG_FILE_NAME, "default.log");
//			size_t		maxFileSize		= getFileSize(sinkConfig, LOGGER_CONFIG_MAX_FILE_SIZE, 10_MB);
//			size_t		maxFiles		= sinkConfig.value(LOGGER_CONFIG_MAX_FILES, 3);
//			bool		rotateOnSession = sinkConfig.value(LOGGER_CONFIG_ROTATE_ON_SESSION, false);
//			addFileOutput(level, maxSkipDuration, fileName, maxFileSize, maxFiles, rotateOnSession);
//		}
//		else if (type == "msvc")
//		{
//			bool checkForDebugger = sinkConfig.value(LOGGER_CONFIG_CHECK_FOR_DEBUGGER, false);
//			auto maxSkipDuration  = std::chrono::microseconds(sinkConfig.value(LOGGER_CONFIG_MAX_SKIP_DURATION, 0));
//			addMSVCOutput(level, checkForDebugger, maxSkipDuration);
//		}
//	}
//}
//
//
//std::shared_ptr<spdlog::logger> getOrCreateLogger(bool drop)
//{
//	static std::once_flag				   flag;
//	static std::shared_ptr<spdlog::logger> logger;
//
//	if (drop)
//	{
//		spdlog::drop_all();
//	}
//
//	std::call_once(flag,
//				   []()
//				   {
//					   auto loggerName = "DefaultLogger";
//					   logger		   = std::make_shared<spdlog::logger>(loggerName);
//					   logger->set_level(spdlog::level::trace);
//					   logger->flush_on(spdlog::level::trace);
//					   spdlog::register_logger(logger);
//					   spdlog::set_default_logger(logger);
//				   });
//
//	return logger;
//}
//
//
//void registerSink(spdlog::sink_ptr sink, std::chrono::microseconds maxSkipDuration, std::unique_ptr<spdlog::formatter> formatter)
//{
//	if (formatter)
//	{
//		sink->set_formatter(std::move(formatter));
//	}
//
//	static std::mutex			sink_mutex;
//	std::lock_guard<std::mutex> lock(sink_mutex);
//
//	auto					   &sinks = LoggerManager::GetInstance()->sinks();
//
//	if (maxSkipDuration > std::chrono::microseconds(0))
//	{
//		auto dupFilter = std::make_shared<spdlog::sinks::dup_filter_sink_mt>(maxSkipDuration);
//		dupFilter->set_level(sink->level());
//		dupFilter->add_sink(sink);
//		sinks.push_back(dupFilter);
//	}
//	else
//	{
//		sinks.push_back(sink);
//	}
//
//	// set sinks for all loggers
//	spdlog::apply_all(
//		[sinks](std::shared_ptr<spdlog::logger> l)
//		{
//			l->sinks().assign(sinks.begin(), sinks.end());
//			l->set_level(spdlog::level::debug);
//		});
//}
//
//
//void log(LogLevel level, const spdlog::source_loc &loc, std::string_view msg)
//{
//	auto logger	  = getOrCreateLogger();
//	auto spdLevel = toSpdLogLevel(level);
//
//	logger->log(loc, spdLevel, msg);
//}
//
//
//void setLoggerName(std::string &name)
//{
//	LoggerManager::GetInstance()->setLoggerName(name);
//}
//
//
//std::string getLoggerName()
//{
//	return LoggerManager::GetInstance()->getLoggerName();
//}



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
