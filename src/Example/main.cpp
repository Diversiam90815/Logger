
#include <iostream>

#include "LoggerWrapper.h"

int main()
{
	// Build a rotating-file and console logger
	auto myLogger = LoggerWrapper()
						.setName("my_app_logger")
						.setLogLevel(LoggerWrapper::Level::Debug)
						.setLogPattern("[%T.%e] [%^%l%$] %v")
						.addRotatingLog("logs/my_app.log", 1024 * 1024 * 5, 3)
						.addConsoleOutput()
						.build();

	// Now, use the logger
	myLogger->trace("This is a trace message");
	myLogger->debug("Hello, SPDLog Debug!");
	myLogger->info("Hello, SPDLog Info!");
	myLogger->warn("Hello, SPDLog Warning!");
	myLogger->error("Hello, SPDLog Error!");

	return 0;
}