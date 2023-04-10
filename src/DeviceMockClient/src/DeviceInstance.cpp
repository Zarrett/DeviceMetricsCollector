#include "DeviceInstance.hpp"
#include "Message.hpp"

#include <atomic>
#include <random>
#include <algorithm>
#include <boost/lexical_cast.hpp>

namespace DeviceMockClient {

	constexpr std::string_view host = "127.0.0.1";

	DeviceInstance::DeviceInstance(boost::asio::io_service& io_service, uint16_t port, uint32_t deviceID, size_t msgNum)
		: m_msgNum(msgNum)
		, m_deviceID(deviceID)
	{
		m_messages = std::make_shared<std::vector<Message>>();
		m_client = std::make_unique<DeviceClient>(io_service, host, port, m_messages);

		run();
	}

	void DeviceInstance::run()
	{
		generateMessages();

		m_client->sendMessages();
	}

	void DeviceInstance::generateMessages()
	{
		constexpr size_t dataSize = 10U;

		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> range(1, 9);

		for (size_t i(0U); i < m_msgNum; ++i)
		{
			std::stringstream messageStr{};

			messageStr << "Measurements: ";

			for (size_t i(0U); i < dataSize; ++i)
			{
				messageStr << std::to_string(range(rng));
			}

			const auto dataLength = messageStr.str().length();

			Message msg((dataLength + sizeof(PacketHeader)));

			auto* header = msg.getHeader();
			auto* data = msg.getBinaryData();

			header->m_commandCode = 1U;
			header->m_dataLenght = dataLength;
			header->m_deviceID = m_deviceID;
			header->m_validationID = ApplicationCode;

			for (size_t i(0U); i < header->m_dataLenght; ++i)
			{
				data[i] = messageStr.str().at(i);
			}

			m_messages->push_back(msg);
		}
	}



} // namespace DeviceMockClient
