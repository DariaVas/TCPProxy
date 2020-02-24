#include "LocalClient.h"
#include <iostream>
#include <functional>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>

LocalClient::LocalClient(int tun_fd, const std::string& server_ip, int server_port):
    m_tun_iodevice(m_tun_svc, tun_fd),
    m_server_sock(m_tcp_svc)
{
    boost::asio::ip::tcp::endpoint ep( boost::asio::ip::address::from_string(server_ip), server_port);
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
       // Do encrypt
        // Do send to server: async_write?)
        m_outcome_server_buffer = m_income_tun_buffer;
        m_server_sock.async_write_some(boost::asio::buffer(m_outcome_server_buffer, m_outcome_server_buffer.size()), boost::bind(&LocalClient::send_to_server_handler, this, _1));
        // Add mechanism to avoid reading loop in case of sigterm got
        m_tun_iodevice.async_read_some(boost::asio::buffer(m_income_tun_buffer), boost::bind(&LocalClient::handle_tun_income_package, this, _1, _2));
    }
}

void LocalClient::handle_server_income_package(const boost::system::error_code &ec, size_t bytes_transferred)
{
    // Hey! POssibly, here we should handle tha case when server sent a lot of data (more than our buffer size)
    //                connection_manager_.stop(shared_from_this());

    if(ec)
    {
        std::cout << "Error '" << ec.message() << "' during asynchronous operation\n";
    }
    else
    {
        std::cout << "Read exactly " << bytes_transferred << " bytes\n";
       // Do dencrypt
        // Do send to tun: async_write?)
        m_outcome_tun_buffer = m_income_server_buffer;
        m_tun_iodevice.async_write_some(boost::asio::buffer(m_outcome_tun_buffer, m_outcome_tun_buffer.size()), boost::bind(&LocalClient::send_to_tun_handler, this, _1));
        // Add mechanism to avoid reading loop in case of sigterm got
        m_server_sock.async_read_some(boost::asio::buffer(m_income_server_buffer), boost::bind(&LocalClient::handle_server_income_package, this, _1, _2));
    }
}


void LocalClient::start_read_from_tun()
{
    m_tun_iodevice.async_read_some(boost::asio::buffer(m_income_tun_buffer), boost::bind(&LocalClient::handle_tun_income_package, this, _1, _2));
    m_tun_svc.run();
}

void LocalClient::start_read_from_server()
{
    m_server_sock.async_read_some(boost::asio::buffer(m_income_server_buffer), boost::bind(&LocalClient::handle_server_income_package, this, _1, _2));
    m_tcp_svc.run();
}

void LocalClient::send_to_server_handler(const boost::system::error_code &error)
{
    if (error)
    {
        std::cout<< "Error when sending package to the server!\n";
    }
}

void LocalClient::send_to_tun_handler(const boost::system::error_code &error)
{
    if (error)
    {
        std::cout<< "Error when sending package to the tun device !\n";
    }
}

void LocalClient::run()
{
    std::thread t_tun(&LocalClient::start_read_from_tun, this);
    std::thread t_server(&LocalClient::start_read_from_tun, this);
    t_tun.join();
    t_server.join();
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
