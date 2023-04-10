#pragma once

#include <vector>
#include <cstdint>
#include <cstring>

namespace DeviceMockClient {

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
			for (size_t i(0U); i < length; ++i)
			{
				m_data.push_back(data[i]);
			}
		}

		explicit Message(const size_t length)
			:m_length(length)
		{
			m_data.resize(length, 0U);
		}

		PacketHeader* getHeader()
		{
			return  reinterpret_cast<PacketHeader*>(m_data.data());
		}

		uint8_t* getBinaryData()
		{
			return (m_data.data() + sizeof(PacketHeader));
		}

		bool isValid(const size_t totalBytesRecvd)
		{
			const auto* header = getHeader();

			if ((!header)
				|| (header->m_validationID != ApplicationCode)
				|| (header->m_dataLenght > MaxBodySize)
				|| (totalBytesRecvd != (header->m_dataLenght + sizeof(PacketHeader))))
			{
				return false;
			}

			return true;
		}

		const std::vector<uint8_t>& getRawData() const
		{
			return m_data;
		}

		const size_t getRawDataLength() const
		{
			return m_length;
		}

	private:
		std::vector<uint8_t> m_data;
		size_t m_length;
	};
} // namespace DeviceMockClient
