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
#include <iostream>
#include "LocalServer.h"

using namespace std;

int main()
{
    char dev [1024]  = "tap0";
     int fd;
     if ( (fd = tun_alloc(dev, IFF_TAP)) < 0){
      perror("call to some_system_call returned a fatal error");
      exit(1);
     }

     try {
       boost::asio::io_service ios;
       Server s(ios, 8080);
       ios.run();
     } catch(std::exception& e) {
       std::cerr << e.what() << std::endl;
     }
    return 0;
}

