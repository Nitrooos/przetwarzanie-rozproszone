#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "You must give an interface name as first argument"
    exit
fi

IF_NAME=$1

# RTM_NEWLINK | RTM_DELLINK messages
ip link add $IF_NAME name eth0 type dummy
ip link del eth0

# RTM_NEWADDR | RTM_DELADDR messages
ip addr add 10.0.0.1/8 dev $IF_NAME
ip addr del 10.0.0.1/8 dev $IF_NAME

# RTM_NEWROUTE | RTM_DELROUTE messages
ip route add 10.0.0.0/24 dev $IF_NAME
ip route del 10.0.0.0/24 dev $IF_NAME

ip route add default via 192.168.1.253
ip route del default via 192.168.1.253

# RTM_NEWNEIGH | RTM_DELNEIGH messages
arp -s 192.168.1.111 12:34:56:78:90:aa
arp -d 192.168.1.111

# RTM_NEWRULE | RTM_DELRULE messages
ip rule add from 10.0.0.1 iif $IF_NAME table default
ip rule del from 10.0.0.1 iif $IF_NAME table default
