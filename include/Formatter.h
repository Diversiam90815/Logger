
#pragma once

#include <spdlog/formatter.h>
//#include <spdlog/pattern_formatter.h>
//#include <spdlog/fmt/bundled/format.h>
//#include <spdlog/details/os.h>
#include <spdlog/details/log_msg.h>



class Formatter : public spdlog::formatter
{
public:
	Formatter();

	void							   format(const spdlog::details::log_msg &msg, spdlog::memory_buf_t &dest) override;

	std::unique_ptr<spdlog::formatter> clone() const override;

private:
	static std::string format_time(const spdlog::log_clock::time_point &tp)
	{
		using namespace std::chrono;

		auto		duration = tp.time_since_epoch();
		auto		secs	 = duration_cast<seconds>(duration);
		auto		ms		 = duration_cast<milliseconds>(duration) - duration_cast<milliseconds>(secs);

		std::time_t t		 = static_cast<std::time_t>(secs.count());
		std::tm		tm_local;

#if defined(_WIN32)
		localtime_s(&tm_local, &t);
#else
		localtime_r(&t, &tm_local);
#endif

		char dateTimeBuf[64];
		std::strftime(dateTimeBuf, sizeof(dateTimeBuf), "%Y-%m-%d %H:%M:%S", &tm_local);

		// Append milliseconds
		// e.g. "2024-12-23 13:13:55.123"
		std::ostringstream oss;
		oss << dateTimeBuf << '.' << std::setw(3) << std::setfill('0') << ms.count();
		return oss.str();
	}


	static std::string get_basename_no_ext(const char *fullpath)
	{
		if (!fullpath || !*fullpath)
			return "Unknown File";

		std::string_view sv(fullpath);
		size_t			 lastSlash = sv.find_last_of("/\\");
		if (lastSlash != std::string_view::npos)
			sv.remove_prefix(lastSlash + 1);

		size_t dotPos = sv.rfind('.');
		if (dotPos != std::string_view::npos)
			sv.remove_suffix(sv.size() - dotPos);

		return std::string(sv);
	}


	std::unique_ptr<spdlog::formatter> mFormatter;
};
