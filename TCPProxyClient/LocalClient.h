#ifndef LOCALCLIENT_H
#define LOCALCLIENT_H

#include <boost/asio.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>

class LocalClient
{
public:
    LocalClient(int tun_fd, const std::string& server_ip, int server_port);
    void run();
private:
    void start_read_from_tun();
    void start_read_from_server();
    void handle_tun_income_package(boost::system::error_code const& ec, size_t bytes_transferred);
    void handle_server_income_package(boost::system::error_code const& ec, size_t bytes_transferred);
    void send_to_server_handler(const boost::system::error_code& error);
    void send_to_tun_handler(const boost::system::error_code& error);
private:
    boost::asio::io_service m_tun_svc;
    boost::asio::io_service m_tcp_svc;
    boost::asio::posix::stream_descriptor m_tun_iodevice; // It will be better to create 2 devices one for reading and one for writing
    std::string m_outcome_tun_buffer;
    std::string m_income_tun_buffer;
    std::string m_outcome_server_buffer;
    std::string m_income_server_buffer;
    boost::asio::ip::tcp::socket m_server_sock;
};

#endif // LOCALCLIENT_H
