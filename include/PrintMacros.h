// #pragma once
// class PrintMacros
//{
// };


#pragma once

#include "LoggerWrapper.h"
#include <spdlog/spdlog.h> // for spdlog::level::...

//
//#define log_info(fmtStr, ...) LOGGER_LOG(LogLevel::Info, fmtStr, ##__VA_ARGS__)



#define LOG_INFO(fmtStr, ...)                                                           \
	do                                                                                  \
	{                                                                                   \
		spdlog::source_loc loc{__FILE__, __LINE__, __FUNCTION__};                       \
		logging::log_with_loc(LogLevel::Info, loc, fmt::format(fmtStr, ##__VA_ARGS__)); \
	} while (0)