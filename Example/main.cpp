
#include <iostream>

#include "LoggerWrapper.h"
#include "PrintMacros.h"


int main()
{
	// Build a rotating-file and console logger
	auto myLogger = LoggerWrapper()
						.setName("my_app_logger")
						.setLogLevel(LoggerWrapper::Level::Debug)
						.addRotatingLog("logs/my_app.log", 1024 * 1024 * 5, 3)
						.addConsoleOutput()
						.build();

	// Now, use the logger
	myLogger->trace("This is a trace message");
	myLogger->debug("Hello, SPDLog Debug!");
	myLogger->info("Hello, SPDLog Info!");
	myLogger->warn("Hello, SPDLog Warning!");
	myLogger->error("Hello, SPDLog Error!");

	std::string asdf = "asdf";
	int iw = 1234;

	info_printf(myLogger, "main", "This is a dbgprintf test : int %i", iw);


	return 0;
}