#pragma once

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

#include <functional>
#include <memory>

using boost::asio::ip::udp;

namespace MetricsCollector {

    namespace PacketParser {
        class Message;
    }

    constexpr size_t MaxPacketSize = 65551U;
    constexpr size_t MinPacketSize = 16U;
    constexpr uint8_t ServerPort = 1111U;

    using MessageReceivedCallback = std::function<void(const std::shared_ptr<PacketParser::Message>& message)>;

    class DeviceServer {
    public:
        DeviceServer(boost::asio::io_service& io_service);

        void subscribeOnMessageReceived(const MessageReceivedCallback& callback);

    private:
        void startReceive();

        void handleReceive(const boost::system::error_code& error,
            std::size_t bytes_transferred);

        void handleSend(const boost::system::error_code& ec,
            std::size_t bytes_transferred);

        void onMessageReceived(size_t bufferSize);
        
        udp::socket m_socket;
        udp::endpoint m_remoteEndpoint;
        std::array<uint8_t, MaxPacketSize> m_recvBuffer;
        size_t m_currentBufferSize = 0;

        MessageReceivedCallback m_messageReceivedCallback;
    };

}  // namespace MetricsCollector