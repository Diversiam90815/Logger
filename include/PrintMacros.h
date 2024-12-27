
#pragma once

#include "LoggerWrapper.h"


#define LOG_TRACE(fmtStr, ...)                                                  \
	do                                                                          \
	{                                                                           \
		spdlog::source_loc loc{__FILE__, __LINE__, __FUNCTION__};               \
		logging::log(LogLevel::Trace, loc, fmt::format(fmtStr, ##__VA_ARGS__)); \
	} while (0)


#define LOG_DEBUG(fmtStr, ...)                                                  \
	do                                                                          \
	{                                                                           \
		spdlog::source_loc loc{__FILE__, __LINE__, __FUNCTION__};               \
		logging::log(LogLevel::Debug, loc, fmt::format(fmtStr, ##__VA_ARGS__)); \
	} while (0)


#define LOG_INFO(fmtStr, ...)                                                  \
	do                                                                         \
	{                                                                          \
		spdlog::source_loc loc{__FILE__, __LINE__, __FUNCTION__};              \
		logging::log(LogLevel::Info, loc, fmt::format(fmtStr, ##__VA_ARGS__)); \
	} while (0)


#define LOG_WARNING(fmtStr, ...)                                               \
	do                                                                         \
	{                                                                          \
		spdlog::source_loc loc{__FILE__, __LINE__, __FUNCTION__};              \
		logging::log(LogLevel::Warn, loc, fmt::format(fmtStr, ##__VA_ARGS__)); \
	} while (0)


#define LOG_ERROR(fmtStr, ...)                                                  \
	do                                                                          \
	{                                                                           \
		spdlog::source_loc loc{__FILE__, __LINE__, __FUNCTION__};               \
		logging::log(LogLevel::Error, loc, fmt::format(fmtStr, ##__VA_ARGS__)); \
	} while (0)


#define LOG_CRITICAL(fmtStr, ...)                                                  \
	do                                                                             \
	{                                                                              \
		spdlog::source_loc loc{__FILE__, __LINE__, __FUNCTION__};                  \
		logging::log(LogLevel::Critical, loc, fmt::format(fmtStr, ##__VA_ARGS__)); \
	} while (0)
