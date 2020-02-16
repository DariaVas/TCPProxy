#include "LocalClient.h"
#include <boost/asio.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>

LocalClient::LocalClient(int tun_fd, const std::string& server_ip, int server_port):
    m_tun_fd(tun_fd),
    m_tun_idevice(m_tun_svc, m_tun_fd),
    m_tun_odevice(m_tun_svc, m_tun_fd),
    m_server_sock(m_tcp_svc)
{
    boost::asio::ip::tcp::endpoint ep( ip::address::from_string(server_ip), server_port);
    m_server_sock.connect(ep); // Here can be an exception, should we catch him?
}

void LocalClient::handle_tun_income_package(boost::system::error_code const& ec, size_t bytes_transferred)
{
    if (ec)
    {
        std::cout << "Error '" << ec.message() << "' during asynchronous operation\n";
    }
    else
    {
        std::cout << "Read exactly " << bytes_transferred << " bytes\n";
       // std::cout << "Data: '";
        //std::cout.write(buffer, bytes_transferred);
        std::cout << "'\n";
       // Do encrypt
        // Do send to server: async_write?)
        char encrypted_msg [2048];
        m_server_sock.async_send(boost::asio::buffer(encrypted_msg), send_to_server_handler);
        // Add mechanism to avoid reading loop in case of sigterm got
        m_tun_iodevice.async_read_some(boost::asio::buffer(income_tun_buffer), handle_tun_income_package);
    }
}


void LocalClient::run_tun_server()
{
    m_tun_idevice.async_read_some(boost::asio::buffer(income_tun_buffer), handle_tun_income_package);
    m_tun_svc.run();
}

void LocalClient::send_to_server_handler(const boost::system::error_code &error, std::size_t bytes_transferred)
{

}

//using boost::asio;
//io_service service;
//ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 2001);
//ip::tcp::socket sock(service);
//sock.async_connect(ep, connect_handler);
//service.run();
//void connect_handler(const boost::system::error_code & ec)
// {
//    // here we know we connected successfully
//    // if ec indicates success
//}
