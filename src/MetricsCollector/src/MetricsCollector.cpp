#include "MetricsCollector.hpp"
#include "PacketParser.hpp"

#include <iostream>

namespace MetricsCollector {

	DeviceMetricsCollector::DeviceMetricsCollector(uint16_t port)
		: m_ioService(std::make_unique<boost::asio::io_service>())
		, m_port(port)
	{
	}

	void DeviceMetricsCollector::startCollecting()
	{
		m_server = std::make_unique<DeviceServer>(*m_ioService, m_port);

		m_server->subscribeOnMessageReceived([this](const std::shared_ptr<PacketParser::Message>& message) {
			onMessageReceived(*message);
		});

		std::cout << "Collecting messages: " << std::endl;

		m_ioService->run();
	}

	void DeviceMetricsCollector::onMessageReceived(
		const PacketParser::Message& message)
	{
		const auto* header = message.getHeader();
		const auto* data = message.getBinaryData();

		const auto now = std::chrono::system_clock::now();
		const std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

		if (m_counterMap.find(header->m_deviceID) != m_counterMap.end())
		{
			m_counterMap[header->m_deviceID] += 1U;
		}
		else
		{
			m_counterMap[header->m_deviceID] = 0U;
		}

		std::stringstream log{};

		log << currentTime
			<< "; DeviceID: "
			<< header->m_deviceID
			<< "; TotalRcv: "
			<< m_counterMap[header->m_deviceID]
			<< "; Message: ";


		for (size_t i(0U); i < header->m_dataLenght; ++i)
		{
			log << static_cast<char>(data[i]);
		}

		log << ";";

		std::cout << log.str() << std::endl;
	}
} // namespace MetricsCollector