#pragma once

#include "Message.hpp"

#include <iostream>
#include <string_view>
#include <deque>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

namespace DeviceMockClient
{
    using boost::asio::ip::udp;
	using boost::asio::ip::address;

    using MSGVector = std::shared_ptr<std::vector<Message>>;

    constexpr size_t MaxPacketSize = 65551U;
    constexpr size_t MinPacketSize = 16U;

    class DeviceClient {
    public:

        DeviceClient() = default;

        DeviceClient(boost::asio::io_service& io_service
            , std::string_view ipAddress
            , std::uint16_t port
            , const MSGVector& messages);

        ~DeviceClient();

        void sendMessages();

    private:
        void doWrite(const Message& msg);
        void handleSendTo(
            const boost::system::error_code& ec,
            std::size_t bytes_transferred);


		udp::socket m_socket;
		std::uint16_t m_port;
		std::string_view m_ipAddress;

        std::vector<uint8_t> m_dataBuffer;
        MSGVector m_messages{};
    };
}
