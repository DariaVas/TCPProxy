#ifndef LOCALCLIENT_H
#define LOCALCLIENT_H
#include <boost/asio.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>
#include <boost/asio/ip/udp.hpp>

class LocalClient
{
public:
    LocalClient(int tun_fd, const std::string& server_ip, int server_port);
    void run();
private:
    void handle_tun_income_package(boost::system::error_code const& ec, size_t bytes_transferred);
    void send_to_server_handler(const boost::system::error_code& error, std::size_t bytes_transferred);
private:
    int m_tun_fd;
    boost::asio::io_service m_tun_svc;
    boost::asio::io_service m_tcp_svc;
    boost::asio::posix::stream_descriptor m_tun_idevice; // It will be better to create 2 devices one for reading and one for writing
    boost::asio::posix::stream_descriptor m_tun_odevice; // It will be better to create 2 devices one for reading and one for writing
    char m_income_tun_buffer[2048];
    boost::asio::ip::tcp::socket m_server_sock;
};

#endif // LOCALCLIENT_H
