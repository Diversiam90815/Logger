/*
==============================================================================
	Module			LoggerManager
	Description		Singleton of the Logger Registry
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

#include "Helper.h"


class LoggerManager
{
public:
	static LoggerManager *GetInstance()
	{
		static LoggerManager *sInstance = nullptr;
		if (nullptr == sInstance)
		{
			sInstance = new LoggerManager();
		}
		return sInstance;
	}

	~LoggerManager() = default;

	std::vector<spdlog::sink_ptr> &sinks() { return mSinks; }

	LogLevel					  &defaultLogLevel() { return mDefaultLogLevel; }

	void						   setLoggerName(std::string &name) { mLoggerName = name; }

	std::string					   getLoggerName() { return mLoggerName; }

private:
	LoggerManager()												   = default;
	LoggerManager(const LoggerManager &)						   = delete;

	LoggerManager				 &operator=(const LoggerManager &) = delete;


	std::vector<spdlog::sink_ptr> mSinks{};
	std::string					  mLoggerName	   = "";
	LogLevel					  mDefaultLogLevel = {LogLevel::Info};
};
