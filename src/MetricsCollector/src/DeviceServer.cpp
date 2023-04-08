#include "DeviceServer.hpp"
#include "PacketParser.hpp"

#include <string>
#include <iostream>

using boost::asio::ip::udp;

namespace MetricsCollector {

DeviceServer::DeviceServer(boost::asio::io_service& io_service)
        : m_socket(io_service, udp::endpoint(udp::v4(), ServerPort))
    {
        startReceive();
    }

void DeviceServer::startReceive() {
   m_socket.async_receive_from(
        boost::asio::buffer(m_recvBuffer), m_remoteEndpoint,
        boost::bind(&DeviceServer::handleReceive, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void DeviceServer::handleReceive(const boost::system::error_code& error,
    std::size_t bytes_transferred) 
{
    if ((MinPacketSize > bytes_transferred) 
        || (bytes_transferred > MaxPacketSize))
    {
        std::cout << "Received packet was of wrong size" << std::endl;
    }

    if (!error || error == boost::asio::error::message_size) {
        m_socket.async_send_to(boost::asio::buffer(m_recvBuffer), m_remoteEndpoint,
            boost::bind(&DeviceServer::handleSend, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }
}

void DeviceServer::handleSend(const boost::system::error_code& ec,
    std::size_t bytes_transferred)
{
    startReceive();
}

void DeviceServer::onMessageReceived(size_t bufferSize)
{
    const auto& message = std::make_shared<PacketParser::Message>(m_recvBuffer.data(), bufferSize);

    if (message->isValid() && m_messageReceivedCallback)
    {
        m_messageReceivedCallback(message);
    }
}

void DeviceServer::subscribeOnMessageReceived(const MessageReceivedCallback& callback)
{
    m_messageReceivedCallback = callback;
}

}  // namespace MetricsCollector