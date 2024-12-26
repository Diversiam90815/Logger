
#include "Formatter.h"


Formatter::Formatter()
{
	mFormatter = std::make_unique<spdlog::pattern_formatter>("%Y-%m-%d %H:%M:%S [%n] %v");
}


void Formatter::format(const spdlog::details::log_msg &msg, spdlog::memory_buf_t &dest)
{
	mFormatter->format(msg, dest);
}


std::unique_ptr<spdlog::formatter> Formatter::clone() const
{
	return std::make_unique<Formatter>();
}
