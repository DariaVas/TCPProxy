#!/bin/bash

IP="172.16.0.1"
TABLE_NAME="TCPClient"

init_tun_device(){
# Init tun device
set -x 

ip addr add 172.16.0.1/24 dev tun0  # give it an ip
ip link set dev tun0 up
}

add_rules(){
# Configure iptables
set -x 
iptables -t mangle -F
iptables -t mangle -X

echo 1 >| /proc/sys/net/ipv4/ip_forward # Actually, i don't know whether i need 2 lines above 
echo 0 >| /proc/sys/net/ipv4/conf/all/rp_filter

iptables -t mangle -I OUTPUT -p tcp --dport 80 -j MARK --set-mark 1
iptables -t nat -A POSTROUTING -p tcp --dport 80 -j MASQUERADE

if ! cat /etc/iproute2/rt_tables | grep -q '^251'
then
        echo '251     '${TABLE_NAME}'' >> /etc/iproute2/rt_tables
fi

ip route flush table ${TABLE_NAME}
ip route add default via ${IP} dev tun0 table ${TABLE_NAME}
ip rule del from all fwmark 1 2>/dev/null
ip rule add from all fwmark 0x1 lookup ${TABLE_NAME}
ip route flush cache
}

remove_rules(){
set -x 
iptables -t mangle -F
iptables -t mangle -X
ip rule del from all fwmark 1 2>/dev/null
}

usage="$(basename "$0") [-h] [-a] [-r] 
where:
    -h  show this help text
    -a  add rules
    -r  remove rules
    -I initialize tun"

if [ "$1" == "-h" ]; then
  echo $usage
  exit 0
 
elif [ "$1" == "-a" ]; then
   init_tun_device
   add_rules
  exit 0 

elif [ "$1" == "-r" ]; then
  remove_rules
  exit 0

elif [ "$1" == "-i" ]; then
  init_tun_device
  exit 0
fi 









