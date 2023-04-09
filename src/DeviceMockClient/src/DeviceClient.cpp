#include "DeviceClient.hpp"
#include "Message.hpp"

#include <string_view>

namespace DeviceMockClient {

    DeviceClient::DeviceClient(boost::asio::io_service& io_service
        , std::string_view ipAddress
        , std::uint16_t port
        , const MSGVector& messages) : 
        
        m_socket(io_service)
        , m_ipAddress{ ipAddress }
        , m_port{ port }
        , m_messages{ messages }
    {
        m_socket.open(udp::v4());
    }

    DeviceClient::~DeviceClient() {
        m_socket.close();
    }

    void DeviceClient::sendMessages() {

        if (m_messages->empty())
        {
            std::cout << "All messages were sent!" << std::endl;
            return;
        }

        auto* header = m_messages->back().getHeader();
        m_dataBuffer = m_messages->back().getRawData();

        std::cout
            << "Sent message from ID: "
            << header->m_deviceID
            << std::endl;

        udp::endpoint remoteEndpoint{ udp::endpoint{address::from_string(m_ipAddress.data()) , m_port} };

        m_socket.async_send_to(boost::asio::buffer(m_dataBuffer), remoteEndpoint,

            boost::bind(&DeviceClient::handleSendTo, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred)
        );
    }

    void DeviceClient::handleSendTo(
        const boost::system::error_code& ec,
        std::size_t bytes_transferred)
    {
        if (ec) {
            std::cout << "error in handle_send_to:" << ec << std::endl;
            m_socket.close();
        }
        else
        {
            // Stupid delay emulation
            std::this_thread::sleep_for(std::chrono::milliseconds(500U));

            m_messages->pop_back();
            sendMessages();
        }
    }

} // namespace DeviceMockClient