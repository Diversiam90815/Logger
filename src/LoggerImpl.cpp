/*
==============================================================================
	Module			LoggerImpl
	Description		Implement all logging functionalities using SPDLOG
==============================================================================
*/

#include "LoggerImpl.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <mutex>
#include <vector>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/formatter.h>
#include <spdlog/details/log_msg.h>


namespace logging
{

// private data structure that holds our spdlog objects
class LoggerImpl::ImplData
{

};


LoggerImpl &LoggerImpl::sInstance()
{
	// TODO: insert return statement here
}

void LoggerImpl::addConsoleOutput(LogLevel level, std::chrono::microseconds maxSkipDuration, const std::string &pattern) {}


void LoggerImpl::addFileOutput(LogLevel level, std::chrono::microseconds maxSkipDuration, const std::string &fileName, size_t maxFileSize, size_t maxFiles, bool rotateOnSession) {}


void LoggerImpl::addMSVCOutput(LogLevel level, bool checkForDebuggerPresent, std::chrono::microseconds maxSkipDuration) {}


void LoggerImpl::initializeLogger(const std::string &configFilePath) {}


void LoggerImpl::log(LogLevel level, const std::string &file, int line, const std::string &function, const std::string &msg) {}


LoggerImpl::LoggerImpl() {}



} // namespace logging
