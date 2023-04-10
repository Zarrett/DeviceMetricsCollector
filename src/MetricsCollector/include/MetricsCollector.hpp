#pragma once

#include "DeviceServer.hpp"

#include <boost/asio.hpp>
#include <unordered_map>

namespace MetricsCollector {

	namespace PacketParser {
		class Message;
	}

	class DeviceMetricsCollector
	{
	public:
		DeviceMetricsCollector(uint16_t port);

		void startCollecting();
	private:
		void onMessageReceived(const PacketParser::Message& message);

		uint16_t m_port;

		std::unordered_map<uint32_t, uint64_t> m_counterMap;
		std::unique_ptr <DeviceServer> m_server;

		std::unique_ptr<boost::asio::io_service> m_ioService;
	};
}
