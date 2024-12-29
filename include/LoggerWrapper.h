/*
  ==============================================================================

	Module			LoggerWrapper
	Description		A builder class to configure and create an SPDLog logger.

  ==============================================================================
*/

#pragma once

#include "Formatter.h"
#include "Helper.h"
#include "LoggerRegistry.h"

using namespace filesize;


namespace logging
{
void addConsoleOutput(LogLevel level, std::chrono::microseconds maxSkipDuration);

void addFileOutput(LogLevel level, std::chrono::microseconds maxSkipDuration, std::string fileName, size_t maxFileSize, size_t maxFiles, bool rotateOnSession);

void addMSVCOutput(LogLevel level, bool checkForDebuggerPresent, std::chrono::microseconds maxSkipDuration);


std::shared_ptr<spdlog::logger> getOrCreateLogger(bool drop = false);

void							registerSink(spdlog::sink_ptr sink, std::chrono::microseconds maxSkipDuration);

void							dropAllAndCreateDefaultLogger();

void							log(LogLevel level, const spdlog::source_loc &loc, std::string_view msg);


/*
 *	@brief		Options for specifying custom sink's features
 */
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


/*
 *	@brief		Options to create a console output sink
 */
struct ConsoleOptions : Options<ConsoleOptions>
{
	ConsoleOptions()							= default;
	ConsoleOptions(const ConsoleOptions &other) = delete;
	~ConsoleOptions()
	{
		logging::addConsoleOutput(level, maxSkipDuration);
	}
};


/*
 *	@brief		Options to create a file output sink
 */
struct FileOptions : Options<FileOptions>
{
	FileOptions()						  = default;
	FileOptions(const FileOptions &other) = delete;
	~FileOptions()
	{
		logging::addFileOutput(level, maxSkipDuration, filename, maxFileSize, maxFiles, rotateOnSession);
	}

	FileOptions &setFilename(std::string filename);
	FileOptions &setMaxFileSize(size_t maxFileSize);
	FileOptions &setMaxFiles(size_t maxFiles);
	FileOptions &setRotateOnSession(bool rotateOnSession);

private:
	std::string filename		= "";
	size_t		maxFileSize		= 10_MB;
	size_t		maxFiles		= 3;
	bool		rotateOnSession = false;
};


/*
 *	@brief		Options to create a MSVC output sink
 */
struct MSVCOptions : Options<MSVCOptions>
{
public:
	MSVCOptions()						  = default;
	MSVCOptions(const MSVCOptions &other) = delete;
	~MSVCOptions()
	{
		logging::addMSVCOutput(level, checkForDebugger, maxSkipDuration);
	}

	MSVCOptions &checkForPresentDebugger(bool check);

private:
	bool checkForDebugger = false;
};


ConsoleOptions addConsoleOutput();

FileOptions	   addFileOutput();

MSVCOptions	   addMSVCOutput();

}; // namespace logging
