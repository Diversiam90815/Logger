//#pragma once
//class PrintMacros
//{
//};


#pragma once

#include "LoggerWrapper.h"
#include <spdlog/fmt/fmt.h>		// for fmt::sprintf
#include <spdlog/spdlog.h>	// for spdlog::level::...


// Each macro calls fmt::sprintf(...) to build a string, then logs it.


//#define dbgprintf(loglevel, moduleflagname, func, ...)\
//	LoggerWrapper::log(loglevel, moduleflagname, func, LoggerWrapper::sprintf(__VA_ARGS__))
//
//
//#define debug_printf(...) 



#define log_printf(logger, loglevel, func, ...) \
	if (logger)                                 \
	logger->log(spdlog::source_loc{__FILE__, __LINE__, func}, loglevel, LoggerWrapper::sprintf(__VA_ARGS__))


	
// Macros for specific log levels
#define trace_printf(logger, func, ...)	   log_printf(logger, spdlog::level::trace, func, __VA_ARGS__)
#define debug_printf(logger, func, ...)	   log_printf(logger, spdlog::level::debug, func, __VA_ARGS__)
#define info_printf(logger, func, ...)	   log_printf(logger, spdlog::level::info, func, __VA_ARGS__)
#define warn_printf(logger, func, ...)	   log_printf(logger, spdlog::level::warn, func, __VA_ARGS__)
#define error_printf(logger, func, ...)	   log_printf(logger, spdlog::level::err, func, __VA_ARGS__)
#define critical_printf(logger, func, ...) log_printf(logger, spdlog::level::critical, func, __VA_ARGS__)