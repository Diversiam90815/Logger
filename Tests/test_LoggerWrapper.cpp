#include <gtest/gtest.h>

#include "LoggerWrapper.h"


TEST(FileSizeLiterals, KilobytesConvertToBytes)
{
	EXPECT_EQ(10_KB, 10ULL * 1024ULL);
}

TEST(FileSizeLiterals, MegabytesConvertToBytes)
{
	EXPECT_EQ(5_MB, 5ULL * 1024ULL * 1024ULL);
}

TEST(LogLevelOrdering, LevelsAreOrderedFromTraceToCritical)
{
	EXPECT_LT(static_cast<int>(LogLevel::Trace), static_cast<int>(LogLevel::Debug));
	EXPECT_LT(static_cast<int>(LogLevel::Debug), static_cast<int>(LogLevel::Info));
	EXPECT_LT(static_cast<int>(LogLevel::Info), static_cast<int>(LogLevel::Warn));
	EXPECT_LT(static_cast<int>(LogLevel::Warn), static_cast<int>(LogLevel::Error));
	EXPECT_LT(static_cast<int>(LogLevel::Error), static_cast<int>(LogLevel::Critical));
}
