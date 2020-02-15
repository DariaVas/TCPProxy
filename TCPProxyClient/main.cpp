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


using namespace std;


int tun_alloc(char *dev, int flags) {

  struct ifreq ifr;
  int fd, err;
  const char *clonedev = "/dev/net/tun";

  /* Arguments taken by the function:
   *
   * char *dev: the name of an interface (or '\0'). MUST have enough
   *   space to hold the interface name if '\0' is passed
   * int flags: interface flags (eg, IFF_TUN etc.)
   */

   /* open the clone device */
   if( (fd = open(clonedev, O_RDWR)) < 0 ) {
     return fd;
   }

   /* preparation of the struct ifr, of type "struct ifreq" */
   memset(&ifr, 0, sizeof(ifr));

   ifr.ifr_flags = flags;   /* IFF_TUN or IFF_TAP, plus maybe IFF_NO_PI */

   if (*dev) {
     /* if a device name was specified, put it in the structure; otherwise,
      * the kernel will try to allocate the "next" device of the
      * specified type */
     strncpy(ifr.ifr_name, dev, IFNAMSIZ);
   }

   /* try to create the device */
   if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ) {
     close(fd);
     return err;
   }

  /* if the operation was successful, write back the name of the
   * interface to the variable "dev", so the caller can know
   * it. Note that the caller MUST reserve space in *dev (see calling
   * code below) */
  strcpy(dev, ifr.ifr_name);

  /* this is the special file descriptor that the caller will use to talk
   * with the virtual interface */
  return fd;
}

int main()
{
    char dev [1024]  = "tun0";
    int fd;
    if ( (fd = tun_alloc(dev, IFF_TUN|IFF_NO_PI)) < 0){
     perror("call to some_system_call returned a fatal error");
     exit(1);

    }
    char buf [2048];
    while(1) {
       int nbytes = read(fd, buf, sizeof(buf));
       printf("Read %d bytes from tun0\n", nbytes);
       std::cout << "Read : " << buf;
       struct ip *iphdr = (struct ip *) buf;
       char src[INET_ADDRSTRLEN];
       char dst[INET_ADDRSTRLEN];
       /* Do whatever with the buf */
       printf("Packet N° %d\n", nbytes);

       u_int8_t protocol = iphdr->ip_p;
       inet_ntop(AF_INET, &(iphdr->ip_src), src, INET_ADDRSTRLEN);
       inet_ntop(AF_INET, &(iphdr->ip_dst), dst, INET_ADDRSTRLEN);

       printf("\nProtocol: %d", protocol);
       printf("\nIP source address: %s", src);
       printf("\nIP dest address: %s", dst);
       printf("\n\n");
     }
    return 0;
}
