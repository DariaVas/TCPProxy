#ifndef LOCALSERVER_H
#define LOCALSERVER_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>


using boost::asio::ip::tcp;


class Session : public std::enable_shared_from_this<Session>
{
public:
  Session(boost::asio::io_service& tcp_svc);

  tcp::socket& get_socket();

  void start(boost::asio::posix::stream_descriptor& tap_io);

  void handle_read(std::shared_ptr<Session>& s,
                   const boost::system::error_code& err,
                   size_t bytes_transferred);
  private:
  tcp::socket m_socket;
  enum { max_length = 1024 };
  char data[max_length];
};

class Server {
public:
  Server(short port, int tap_fd);
  void handle_accept(std::shared_ptr<Session> session,
                     const boost::system::error_code& err);
private:
  boost::asio::io_service m_tcp_svc;
  boost::asio::io_service m_tap_svc;
  boost::asio::posix::stream_descriptor m_tap_iodevice;
  tcp::acceptor acceptor;
};
//class LocalServer
//{
//public:
//    LocalServer(int tap_fd, int port);
//    void run();
//private:
//    void start_read_from_tap();
//    void start_read_from_client();
//    void handle_tap_income_package(boost::system::error_code const& ec, size_t bytes_transferred);
//    void handle_client_income_package(boost::system::error_code const& ec, size_t bytes_transferred);
//    void send_to_client_handler(const boost::system::error_code& error);
//    void send_to_tap_handler(const boost::system::error_code& error);
//private:
//    int m_tap_fd;
//    boost::asio::io_service m_tap_svc;
//    boost::asio::io_service m_tcp_svc;
//    boost::asio::posix::stream_descriptor m_tap_iodevice; // It will be better to create 2 devices one for reading and one for writing
//    std::string m_outcome_tap_buffer;
//    std::string m_income_tap_buffer;
//    std::string m_outcome_server_buffer;
//    std::string m_income_server_buffer;
//    boost::asio::ip::tcp::socket m_server_sock;
//};


#endif // LOCALSERVER_H
