#pragma once

#include <cstring>

namespace MetricsCollector::PacketParser {

	constexpr uint32_t ApplicationCode = 12345U;

	constexpr size_t MaxBodySize = 65535U;

	struct PacketHeader {
		uint32_t m_commandCode;
		uint32_t m_deviceID;
		uint32_t m_validationID;
		uint32_t m_dataLenght;
	};

	class Message
	{
	public:
		Message(const uint8_t* data, const size_t length) 
			:m_length(length)
		{
			m_data = new uint8_t[m_length];
			std::memcpy(m_data, data, m_length);
		}

		~Message()
		{
			if (m_data)
			{
				delete m_data;
			}
		}

		const PacketHeader* getHeader() const
		{
			return m_data ? reinterpret_cast<PacketHeader*>(m_data) : nullptr;
		}

		const uint8_t* getBinaryData() const
		{
			return m_data ? (m_data + sizeof(PacketHeader)) : nullptr;
		}

		bool isValid()
		{
			const auto* header = getHeader();

			if ((!header) 
				|| (header->m_validationID != ApplicationCode) 
				|| (header->m_dataLenght > MaxBodySize))
			{
				return false;
			}

			return true;
		}

	private:
		uint8_t* m_data = nullptr;
		size_t m_length;
	};

} // namespace MetricsCollector