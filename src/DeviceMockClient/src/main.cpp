#include "DeviceInstance.hpp"

#include <iostream>
#include <algorithm>
#include <chrono>

void run(uint32_t deviceID, uint16_t port, size_t numberOfMessages)
{
	std::cout << "Running on port: " << port << std::endl;
	std::cout << "DeviceID: " << deviceID << std::endl;

	boost::asio::io_service io_service;

	auto instance = std::make_unique<DeviceMockClient::DeviceInstance>(io_service,
		port, deviceID, numberOfMessages);

	io_service.run();
}

int main(int argc, char* argv[])
{
	uint16_t port = 8888U;
	uint32_t deviceID = 1U;
	size_t numberOfMessages = 100U;

	if (argc > 1U)
	{
		try {
			port = std::stoi(argv[1U]);
		}
		catch (const std::exception& ex)
		{
			std::cout << "Conversion failed: " << ex.what() << std::endl;
		}
	}

	if (argc > 2U)
	{
		try {
			deviceID = std::stoi(argv[2U]);
		}
		catch (const std::exception& ex)
		{
			std::cout << "Conversion failed: " << ex.what() << std::endl;
		}
	}

	if (argc > 3U)
	{
		try {
			numberOfMessages = std::stoi(argv[3U]);
		}
		catch (const std::exception& ex)
		{
			std::cout << "Conversion failed: " << ex.what() << std::endl;
		}
	}

	run(deviceID, port, numberOfMessages);

	return 0;
}