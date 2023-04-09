#pragma once

#include "DeviceClient.hpp"

namespace DeviceMockClient {
	class DeviceInstance
	{
	public:
		DeviceInstance(boost::asio::io_service& io_service, uint16_t port, uint32_t deviceID, size_t msgNum);

	private:
		void run();
		void generateMessages();
		
		std::unique_ptr<DeviceClient> m_client;
		
		uint32_t m_deviceID;
		size_t m_msgNum;

		MSGVector m_messages{};
	};

} // namespace DeviceMockClient