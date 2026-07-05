#include <gtest/gtest.h>

#include "Formatter.h"


namespace
{

std::string formatMessage(spdlog::level::level_enum level, const char *filename, int line, const char *funcname, const char *payload)
{
	spdlog::source_loc			  loc(filename, line, funcname);
	spdlog::details::log_msg	  msg(loc, "test_logger", level, payload);

	Formatter					  formatter;
	spdlog::memory_buf_t		  dest;
	formatter.format(msg, dest);

	return std::string(dest.data(), dest.size());
}

} // namespace


TEST(Formatter, IncludesLogLevel)
{
	auto line = formatMessage(spdlog::level::info, "/src/Foo.cpp", 10, "doWork", "hello");
	EXPECT_NE(line.find("info"), std::string::npos);
}

TEST(Formatter, IncludesPayloadMessage)
{
	auto line = formatMessage(spdlog::level::warn, "/src/Foo.cpp", 10, "doWork", "something happened");
	EXPECT_NE(line.find("something happened"), std::string::npos);
}

TEST(Formatter, ExtractsBasenameWithoutExtensionFromSourcePath)
{
	auto line = formatMessage(spdlog::level::err, "/some/nested/path/MyModule.cpp", 42, "run", "boom");
	EXPECT_NE(line.find("MyModule"), std::string::npos);
	EXPECT_EQ(line.find("MyModule.cpp"), std::string::npos);
}

TEST(Formatter, HandlesWindowsStyleSourcePath)
{
	auto line = formatMessage(spdlog::level::debug, "C:\\project\\src\\Other.cpp", 7, "step", "tick");
	EXPECT_NE(line.find("Other"), std::string::npos);
}

TEST(Formatter, EndsWithNewline)
{
	auto line = formatMessage(spdlog::level::trace, "/src/Foo.cpp", 1, "f", "msg");
	ASSERT_FALSE(line.empty());
	EXPECT_EQ(line.back(), '\n');
}

TEST(Formatter, CloneProducesEquivalentOutput)
{
	spdlog::source_loc		  loc("/src/Foo.cpp", 10, "doWork");
	spdlog::details::log_msg msg(loc, "test_logger", spdlog::level::info, "hello");

	Formatter				  original;
	auto					  cloned = original.clone();
	ASSERT_NE(cloned, nullptr);

	spdlog::memory_buf_t originalDest;
	spdlog::memory_buf_t clonedDest;
	original.format(msg, originalDest);
	cloned->format(msg, clonedDest);

	EXPECT_EQ(std::string(originalDest.data(), originalDest.size()), std::string(clonedDest.data(), clonedDest.size()));
}
