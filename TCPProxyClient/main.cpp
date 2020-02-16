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
