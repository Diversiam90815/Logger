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

enum class LogLevel
{
	Trace,
	Debug,
	Info,
	Warn,
	Error,
	Critical
};


class LoggerRegistry
{
public:
	static LoggerRegistry &sInstance()
	{
		static LoggerRegistry instance;
		return instance;
	}

	std::vector<spdlog::sink_ptr> &sinks()
	{
		return mSinks;
	}

	LogLevel &defaultLogLevel()
	{
		return mDefaultLogLevel;
	}

	//std::string &mainModuleName()
	//{
	//	return mMainModuleName;
	//}

private:
	LoggerRegistry()												= default;
	~LoggerRegistry()												= default;
	LoggerRegistry(const LoggerRegistry &)							= delete;
	LoggerRegistry				 &operator=(const LoggerRegistry &) = delete;

	std::vector<spdlog::sink_ptr> mSinks;
	LogLevel					  mDefaultLogLevel = {LogLevel::Info};
	//std::string					  mMainModuleName;
};


namespace logging
{


//inline std::string mainModuleName = "";



void			   addConsoleOutput(LogLevel level, std::chrono::microseconds maxSkipDuration);

void			   addFileOutput(LogLevel level, std::chrono::microseconds maxSkipDuration, std::string fileName, size_t maxFileSize, size_t maxFiles, bool rotateOnSession);

//const std::string &getMainModuleName();


std::shared_ptr<spdlog::logger> getOrCreateLogger(bool drop = false);


void							registerSink(spdlog::sink_ptr sink, std::chrono::microseconds maxSkipDuration);

void							dropAllAndCreateDefaultLogger();

static std::string				sprintf(const char *format, ...);


void							log(LogLevel lvl, std::string_view module, std::string_view func, std::string_view msg);

void							log_with_loc(LogLevel level, const spdlog::source_loc &loc, std::string_view msg);


//
//#define LOGGER_MAIN_MODULE_NAME			   (logging::mainModuleName.empty() ? logging::getMainModuleName() : logging::mainModuleName)
//
//#define LOGGER_FUNCTION					   static_cast<const char *>(__FUNCTION__)
//
//#define LOGGER_SET_MODULE_NAME(moduleName) logging::mainModuleName = #moduleName
//


template <typename Output>
struct Options
{
public:
	Output &setLevel(LogLevel level) noexcept
	{
		this->level = level;
		return static_cast<Output &>(*this);
	}

	Output &setMaxSkipDuration(std::chrono::microseconds maxSkipDuration) noexcept
	{
		this->maxSkipDuration = maxSkipDuration;
		return static_cast<Output &>(*this);
	}

protected:
	LogLevel				  level = LogLevel::Info;
	std::chrono::microseconds maxSkipDuration;
};

struct ConsoleOptions : Options<ConsoleOptions>
{
	ConsoleOptions()							= default;
	ConsoleOptions(const ConsoleOptions &other) = delete;
	~ConsoleOptions()
	{
		logging::addConsoleOutput(level, maxSkipDuration);
	}
};


struct FileOptions : Options<FileOptions>
{
	FileOptions()						  = default;
	FileOptions(const FileOptions &other) = delete;

	FileOptions &setFilename(std::string filename)
	{
		this->filename = filename;
		return *this;
	}

	FileOptions &setMaxFileSize(size_t maxFileSize)
	{
		this->maxFileSize = maxFileSize;
		return *this;
	}

	FileOptions &setMaxFiles(size_t maxFiles)
	{
		this->maxFiles = maxFiles;
		return *this;
	}

	FileOptions &setRotateOnSession(bool rotateOnSession)
	{
		this->rotateOnSession = rotateOnSession;
		return *this;
	}

	~FileOptions()
	{
		logging::addFileOutput(level, maxSkipDuration, filename, maxFileSize, maxFiles, rotateOnSession);
	}

private:
	std::string filename		= "";
	size_t		maxFileSize		= 0;
	size_t		maxFiles		= 3;
	bool		rotateOnSession = false;
};



ConsoleOptions addConsoleOutput();

FileOptions	   addFileOutput();


}; // namespace logging
