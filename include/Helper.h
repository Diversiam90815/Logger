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

LogLevel toLogLevel(const std::string &level)
{
	if (level == "trace")
		return LogLevel::Trace;
	if (level == "debug")
		return LogLevel::Debug;
	if (level == "info")
		return LogLevel::Info;
	if (level == "warn")
		return LogLevel::Warn;
	if (level == "error")
		return LogLevel::Error;
	if (level == "critical")
		return LogLevel::Critical;
	throw std::invalid_argument("Invalid log level: " + level);
}


// Function to trim whitespaces from a string
std::string trim(const std::string &s)
{
	size_t start = s.find_first_not_of(" \t\n\r");
	size_t end	 = s.find_last_not_of(" \t\n\r");
	return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

unsigned long long parseFileSize(const std::string &sizeStr)
{
	std::string s	= trim(sizeStr); // Trim in case of extra whitespaces

	// Extract numeric part
	size_t		pos = 0;
	while (pos < s.size() && (std::isdigit(s[pos]) || s[pos] == '.'))
	{
		++pos;
	}

	if (pos == 0)
		throw std::invalid_argument("No numeric part found in file size string : " + sizeStr);

	unsigned long long numberValue = std::stoull(s.substr(0, pos)); // Extract numeric value


	// Extract the unit
	std::string		   unit		   = trim(s.substr(pos));
	unit.erase(std::remove(unit.begin(), unit.end(), '_'), unit.end());

	// Convert unit to upper case
	for (auto &ch : unit)
	{
		ch = std::toupper(ch);
	}

	// Get correct unit
	if (unit == "KB")
		return numberValue * 1024ULL;
	else if (unit == "MB")
		return numberValue * 1024ULL * 1024ULL;
	else if (unit == "GB")
		return numberValue * 1024ULL * 1024ULL * 1024ULL;
	else if (unit.empty())
		return numberValue; // No unit means bytes
	else
		throw std::invalid_argument("Unkown file size unit: " + unit);
}


unsigned long long getFileSize(const json &j, const std::string &key, unsigned long long defaultValue)
{
	if (j.contains(key))
	{
		if (j[key].is_number_unsigned())
		{
			return j[key].get<unsigned long long>();
		}
		else if (j[key].is_string())
		{
			return parseFileSize(j[key].get<std::string>());
		}
		else
		{
			throw std::runtime_error("Invalid type for file size in config for key: " + key);
		}
	}
	return defaultValue;
}




} // anonymous namespace