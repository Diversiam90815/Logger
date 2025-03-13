/*
==============================================================================
	Module			Helper
	Description		Constants and Helper methods needed throughout the project
==============================================================================
*/ 

#pragma once

enum class LogLevel
{
	Trace,
	Debug,
	Info,
	Warn,
	Error,
	Critical
};


namespace filesize
{
inline constexpr unsigned long long operator""_KB(unsigned long long value)
{
	return value * 1'024;
}

inline constexpr unsigned long long operator""_MB(unsigned long long value)
{
	return value * 1'024 * 1_KB;
}
} // namespace filesize


namespace
{
spdlog::level::level_enum toSpdLogLevel(LogLevel level)
{
	switch (level)
	{
	case LogLevel::Trace: return spdlog::level::trace;
	case LogLevel::Debug: return spdlog::level::debug;
	case LogLevel::Info: return spdlog::level::info;
	case LogLevel::Warn: return spdlog::level::warn;
	case LogLevel::Error: return spdlog::level::err;
	case LogLevel::Critical: return spdlog::level::critical;
	default: return spdlog::level::off;
	}
}
} // anonymous namespace