
#include "Formatter.h"


Formatter::Formatter()
{
}


void Formatter::format(const spdlog::details::log_msg &msg, spdlog::memory_buf_t &dest)
{
	auto		time_str	= format_time(msg.time);
	auto	   &level_str	= spdlog::level::to_string_view(msg.level);
	auto		thread_id	= msg.thread_id;
	auto		fileOnly	= get_basename_no_ext(msg.source.filename);

	if (msg.logger_name.size() != 0)
	{
		fileOnly.assign(msg.logger_name.data(), msg.logger_name.size());
	}

	auto		  funcname = (msg.source.funcname ? msg.source.funcname : "");

	std::string	  user_msg(msg.payload.begin(), msg.payload.end());

	constexpr int TIME_WIDTH   = 23; // "YYYY-MM-DD HH:MM:SS.mmm" ~ 23 chars
	constexpr int THREAD_WIDTH = 6;
	constexpr int LEVEL_WIDTH  = 5;
	constexpr int MODULE_WIDTH = 18;
	constexpr int FUNC_WIDTH   = 25;

	auto formatted_line		   = fmt::format("{:<{}} {:>{}} {:<{}} {:<{}} {:<{}} {}", time_str, TIME_WIDTH, thread_id, THREAD_WIDTH, level_str, LEVEL_WIDTH, fileOnly, MODULE_WIDTH,
									  funcname, FUNC_WIDTH, user_msg);

	fmt::format_to(std::back_inserter(dest), "{}\n", formatted_line);
}


std::unique_ptr<spdlog::formatter> Formatter::clone() const
{
	return std::make_unique<Formatter>();
}
