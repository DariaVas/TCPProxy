#include "LocalServer.h"


Session::Session(boost::asio::io_service &tcp_svc): m_socket(tcp_svc)
{

}

tcp::socket& Session::get_socket()
{
  return m_socket;
}

void Session::start(boost::asio::posix::stream_descriptor& tap_io)
{
  m_socket.async_read_some(
      boost::asio::buffer(data, max_length),
      boost::bind(&Session::handle_read, this,
                  shared_from_this(),
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
}


void Session::handle_read(std::shared_ptr<Session>& s,
                 const boost::system::error_code& err,
                 size_t bytes_transferred)
{
  if (!err) {
    std::cout << "recv: " + test << std::endl;
    // write to tap buffer
    m_socket.async_read_some(
        boost::asio::buffer(data, max_length),
        boost::bind(&Session::handle_read, this,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
  } else {
    std::cerr << "err (recv): " << err.message() << std::endl;
  }
}


Server::Server(short port, int tap_fd) : m_tap_iodevice(m_tap_svc, tap_fd),  acceptor(tcp_svc, tcp::endpoint(tcp::v4(), port))
{
  std::shared_ptr<Session> session = std::make_shared<Session>(tcp_svc);
  acceptor.async_accept(session->get_socket(),
                        boost::bind(&Server::handle_accept, this,
                                    session,
                                    boost::asio::placeholders::error));
}


void Server::handle_accept(std::shared_ptr<Session> session,
                   const boost::system::error_code& err)
{
  if (!err) {
    session->start(m_tap_iodevice);
    session = std::make_shared<Session>(m_tcp_svc);
    acceptor.async_accept(session->get_socket(),
                          boost::bind(&Server::handle_accept, this, session,
                                      boost::asio::placeholders::error));
  }
  else {
    std::cerr << "err: " + err.message() << std::endl;
    session.reset();
  }
}
