
#pragma once

#include <spdlog/formatter.h>
#include <spdlog/pattern_formatter.h>

class Formatter : public spdlog::formatter
{
public:
	Formatter();

	void					   format(const spdlog::details::log_msg &msg, spdlog::memory_buf_t &dest) override;

	std::unique_ptr<spdlog::formatter> clone() const override;

private:
	std::unique_ptr<spdlog::formatter> mFormatter;
};
