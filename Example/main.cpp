
#include <iostream>

#include "LoggerWrapper.h"
#include "PrintMacros.h"

#define LOG_MODULE_NAME "MainFileExample"



static void LoggingMessagesForTest()
{
	LOG_INFO("This is a test log from an other method!");
	LOG_INFO("Integer :  {}!", 12344);
}


static void LogErrors()
{
	std::string error = "Example of Error!";
	LOG_ERROR("This is an error message : {}", error);
}


int main()
{
	using namespace filesize;

	// Configure logging outputs
	logging::addFileOutput().setFilename("TestLog.log").setLevel(LogLevel::Info).setMaxFileSize(20_MB).setRotateOnSession(true);

	// Maybe also add console output at a different level
	logging::addConsoleOutput().setLevel(LogLevel::Debug);


	int someInteger = 42;

	LOG_INFO("Hello. This is some integer: {}", someInteger);

	LoggingMessagesForTest();
	LogErrors();

	return 0;
}
