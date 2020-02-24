#include <iostream>
#include "LocalClient.h"
#include <boost/asio.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>
#include <boost/function.hpp>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <linux/if_tun.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>
#include <stdarg.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include "network_utils.h"

using namespace std;

int main()
{
    // Hey, we don't care about tun fd because it will be realesed after program death
    char dev [1024]  = "tun0";
    int fd;
    if ( (fd = tun_alloc(dev, IFF_TUN|IFF_NO_PI)) < 0){
     perror("call to some_system_call returned a fatal error");
     exit(1);
    }
    std::cout << dev << " device was created\n";
    std::string server_ip = "127.0.0.1";
    int server_port = 8991;
    LocalClient client(fd, server_ip, server_port);
    client.run();
//    char buf [2048];
//    while(1) {
//       int nbytes = read(fd, buf, sizeof(buf));
//       printf("Read %d bytes from tun0\n", nbytes);
//       std::cout << "Read : " << buf;
//       struct ip *iphdr = (struct ip *) buf;
//       char src[INET_ADDRSTRLEN];
//       char dst[INET_ADDRSTRLEN];
//       /* Do whatever with the buf */
//       printf("Packet N° %d\n", nbytes);

//       u_int8_t protocol = iphdr->ip_p;
//       inet_ntop(AF_INET, &(iphdr->ip_src), src, INET_ADDRSTRLEN);
//       inet_ntop(AF_INET, &(iphdr->ip_dst), dst, INET_ADDRSTRLEN);

//       printf("\nProtocol: %d", protocol);
//       printf("\nIP source address: %s", src);
//       printf("\nIP dest address: %s", dst);
//       printf("\n\n");
//     }



//    boost::asio::io_service io_svc;
//    boost::asio::posix::stream_descriptor iodevice(io_svc, fd);

//    char buffer[2048];
//    boost::function<void(boost::system::error_code const&, size_t)> callback;
//    callback = [&](boost::system::error_code const& ec, size_t bytes_transferred) {
//            if (ec)
//            {
//                std::cout << "Error '" << ec.message() << "' during asynchronous operation\n";
//            }
//            else
//            {
//                std::cout << "Read exactly " << bytes_transferred << " bytes\n";
//               // std::cout << "Data: '";
//                //std::cout.write(buffer, bytes_transferred);
//                std::cout << "'\n";

//                iodevice.async_read_some(boost::asio::buffer(buffer), callback);
//            }
//        };

//    iodevice.async_read_some(boost::asio::buffer(buffer), callback);

//    io_svc.run();

    return 0;
}

/**
IniConfig()
{
    read();
    get(key);
}


**/
/**
LocalCLient()
{
    run()
    {
        create one thread for reading from tun and one for reading from socket
    }

    process_incomming_tun_packages()
    {
        run()
    }

    process_incomming_server_packages()
    {
        run()
    }

    handle_tun_income_package()
    {
       read package
       encrypt_package
       socket.async_write()
       read_some()
    }

    handle_server_income_package()
    {
       read package
       dencrypt_package
       tun.async_write()
       read_some()
    }
}

***/



//class client
//{
//public:
//  client(boost::asio::io_context& io_context)
//    : stopped_(false),
//      socket_(io_context),
//      deadline_(io_context),
//      heartbeat_timer_(io_context)
//  {
//  }

//  // Called by the user of the client class to initiate the connection process.
//  // The endpoints will have been obtained using a tcp::resolver.
//  void start(tcp::resolver::results_type endpoints)
//  {
//    // Start the connect actor.
//    endpoints_ = endpoints;
//    start_connect(endpoints_.begin());

//    // Start the deadline actor. You will note that we're not setting any
//    // particular deadline here. Instead, the connect and input actors will
//    // update the deadline prior to each asynchronous operation.
//    deadline_.async_wait(boost::bind(&client::check_deadline, this));
//  }

//  // This function terminates all the actors to shut down the connection. It
//  // may be called by the user of the client class, or by the class itself in
//  // response to graceful termination or an unrecoverable error.
//  void stop()
//  {
//    stopped_ = true;
//    boost::system::error_code ignored_ec;
//    socket_.close(ignored_ec);
//    deadline_.cancel();
//    heartbeat_timer_.cancel();
//  }

//private:
//  void start_connect(tcp::resolver::results_type::iterator endpoint_iter)
//  {
//    if (endpoint_iter != endpoints_.end())
//    {
//      std::cout << "Trying " << endpoint_iter->endpoint() << "...\n";

//      // Set a deadline for the connect operation.
//      deadline_.expires_after(boost::asio::chrono::seconds(60));

//      // Start the asynchronous connect operation.
//      socket_.async_connect(endpoint_iter->endpoint(),
//          boost::bind(&client::handle_connect,
//            this, _1, endpoint_iter));
//    }
//    else
//    {
//      // There are no more endpoints to try. Shut down the client.
//      stop();
//    }
//  }

//  void handle_connect(const boost::system::error_code& ec,
//      tcp::resolver::results_type::iterator endpoint_iter)
//  {
//    if (stopped_)
//      return;

//    // The async_connect() function automatically opens the socket at the start
//    // of the asynchronous operation. If the socket is closed at this time then
//    // the timeout handler must have run first.
//    if (!socket_.is_open())
//    {
//      std::cout << "Connect timed out\n";

//      // Try the next available endpoint.
//      start_connect(++endpoint_iter);
//    }

//    // Check if the connect operation failed before the deadline expired.
//    else if (ec)
//    {
//      std::cout << "Connect error: " << ec.message() << "\n";

//      // We need to close the socket used in the previous connection attempt
//      // before starting a new one.
//      socket_.close();

//      // Try the next available endpoint.
//      start_connect(++endpoint_iter);
//    }

//    // Otherwise we have successfully established a connection.
//    else
//    {
//      std::cout << "Connected to " << endpoint_iter->endpoint() << "\n";

//      // Start the input actor.
//      start_read();

//      // Start the heartbeat actor.
//      start_write();
//    }
//  }

//  void start_read()
//  {
//    // Set a deadline for the read operation.
//    deadline_.expires_after(boost::asio::chrono::seconds(30));

//    // Start an asynchronous operation to read a newline-delimited message.
//    boost::asio::async_read_until(socket_,
//        boost::asio::dynamic_buffer(input_buffer_), '\n',
//        boost::bind(&client::handle_read, this, _1, _2));
//  }

//  void handle_read(const boost::system::error_code& ec, std::size_t n)
//  {
//    if (stopped_)
//      return;

//    if (!ec)
//    {
//      // Extract the newline-delimited message from the buffer.
//      std::string line(input_buffer_.substr(0, n - 1));
//      input_buffer_.erase(0, n);

//      // Empty messages are heartbeats and so ignored.
//      if (!line.empty())
//      {
//        std::cout << "Received: " << line << "\n";
//      }

//      start_read();
//    }
//    else
//    {
//      std::cout << "Error on receive: " << ec.message() << "\n";

//      stop();
//    }
//  }

//  void start_write()
//  {
//    if (stopped_)
//      return;

//    // Start an asynchronous operation to send a heartbeat message.
//    boost::asio::async_write(socket_, boost::asio::buffer("\n", 1),
//        boost::bind(&client::handle_write, this, _1));
//  }

//  void handle_write(const boost::system::error_code& ec)
//  {
//    if (stopped_)
//      return;

//    if (!ec)
//    {
//      // Wait 10 seconds before sending the next heartbeat.
//      heartbeat_timer_.expires_after(boost::asio::chrono::seconds(10));
//      heartbeat_timer_.async_wait(boost::bind(&client::start_write, this));
//    }
//    else
//    {
//      std::cout << "Error on heartbeat: " << ec.message() << "\n";

//      stop();
//    }
//  }

//  void check_deadline()
//  {
//    if (stopped_)
//      return;

//    // Check whether the deadline has passed. We compare the deadline against
//    // the current time since a new asynchronous operation may have moved the
//    // deadline before this actor had a chance to run.
//    if (deadline_.expiry() <= steady_timer::clock_type::now())
//    {
//      // The deadline has passed. The socket is closed so that any outstanding
//      // asynchronous operations are cancelled.
//      socket_.close();

//      // There is no longer an active deadline. The expiry is set to the
//      // maximum time point so that the actor takes no action until a new
//      // deadline is set.
//      deadline_.expires_at(steady_timer::time_point::max());
//    }

//    // Put the actor back to sleep.
//    deadline_.async_wait(boost::bind(&client::check_deadline, this));
//  }

//private:
//  bool stopped_;
//  tcp::resolver::results_type endpoints_;
//  tcp::socket socket_;
//  std::string input_buffer_;
//  steady_timer deadline_;
//  steady_timer heartbeat_timer_;
//};

//int main(int argc, char* argv[])
//{
//  try
//  {
//    if (argc != 3)
//    {
//      std::cerr << "Usage: client <host> <port>\n";
//      return 1;
//    }

//    boost::asio::io_context io_context;
//    tcp::resolver r(io_context);
//    client c(io_context);

//    c.start(r.resolve(argv[1], argv[2]));

//    io_context.run();
//  }
//  catch (std::exception& e)
//  {
//    std::cerr << "Exception: " << e.what() << "\n";
//  }

//  return 0;
//}
