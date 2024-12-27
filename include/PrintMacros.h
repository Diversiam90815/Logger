
#pragma once

#include "LoggerWrapper.h"
#include <spdlog/spdlog.h> // for spdlog::level::...


#define LOG_INFO(fmtStr, ...)                                                  \
	do                                                                         \
	{                                                                          \
		spdlog::source_loc loc{__FILE__, __LINE__, __FUNCTION__};              \
		logging::log(LogLevel::Info, loc, fmt::format(fmtStr, ##__VA_ARGS__)); \
	} while (0)