#include "MetricsCollector.hpp"
#include <iostream>
#include <thread>
#include <chrono>

int main(int argc, char* argv[])
{
	uint16_t port = 8888U;

	if (argc > 1U)
	{
		try {
			port = std::stoi(argv[1]);
		}
		catch (const std::exception& ex)
		{
			std::cout << "Conversion failed: " << ex.what() << std::endl;
		}
	}

	std::cout << "Started collecting on port: " << port << std::endl;

	MetricsCollector::DeviceMetricsCollector collector{ port };
	collector.startCollecting();

	return 0;
}