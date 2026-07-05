#include <gtest/gtest.h>

#include "Helper.h"


TEST(Trim, RemovesLeadingAndTrailingWhitespace)
{
	EXPECT_EQ(trim("  hello world  "), "hello world");
	EXPECT_EQ(trim("\t\nhello\r\n"), "hello");
}

TEST(Trim, LeavesInnerWhitespaceUntouched)
{
	EXPECT_EQ(trim("  hello   world  "), "hello   world");
}

TEST(Trim, HandlesEmptyAndAllWhitespaceStrings)
{
	EXPECT_EQ(trim(""), "");
	EXPECT_EQ(trim("   \t\n  "), "");
}

TEST(Trim, LeavesStringWithoutWhitespaceUntouched)
{
	EXPECT_EQ(trim("hello"), "hello");
}


TEST(ParseFileSize, ParsesKilobytes)
{
	EXPECT_EQ(parseFileSize("10KB"), 10ULL * 1024ULL);
}

TEST(ParseFileSize, ParsesMegabytes)
{
	EXPECT_EQ(parseFileSize("10MB"), 10ULL * 1024ULL * 1024ULL);
}

TEST(ParseFileSize, ParsesGigabytes)
{
	EXPECT_EQ(parseFileSize("2GB"), 2ULL * 1024ULL * 1024ULL * 1024ULL);
}

TEST(ParseFileSize, ParsesUnderscoreSeparatedUnit)
{
	EXPECT_EQ(parseFileSize("10_MB"), 10ULL * 1024ULL * 1024ULL);
}

TEST(ParseFileSize, ParsesLowerCaseUnit)
{
	EXPECT_EQ(parseFileSize("512kb"), 512ULL * 1024ULL);
}

TEST(ParseFileSize, TreatsMissingUnitAsBytes)
{
	EXPECT_EQ(parseFileSize("1024"), 1024ULL);
}

TEST(ParseFileSize, TrimsSurroundingWhitespace)
{
	EXPECT_EQ(parseFileSize("  10 MB  "), 10ULL * 1024ULL * 1024ULL);
}

TEST(ParseFileSize, ThrowsWhenNoNumericPartPresent)
{
	EXPECT_THROW(parseFileSize("MB"), std::invalid_argument);
}

TEST(ParseFileSize, ThrowsOnUnknownUnit)
{
	EXPECT_THROW(parseFileSize("10TB"), std::invalid_argument);
}
