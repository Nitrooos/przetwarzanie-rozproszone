#!/bin/zsh

# RTM_NEWLINK | RTM_DELLINK messages
ip link add enp3s0f1 name eth0 type dummy
ip link del eth0

# RTM_NEWADDR | RTM_DELADDR messages
ip addr add 10.0.0.1/8 dev enp3s0f1
ip addr del 10.0.0.1/8 dev enp3s0f1

# RTM_NEWROUTE | RTM_DELROUTE messages
ip route add 192.168.1.0/24 dev enp3s0f1
ip route del 192.168.1.0/24 dev enp3s0f1

ip route add default via 192.168.1.254/24
ip route del default via 192.168.1.254/24

# RTM_NEWNEIGH | RTM_DELNEIGH messages
arp -s 192.168.1.111 12:34:56:78:90:aa
arp -d 192.168.1.111

# RTM_NEWRULE | RTM_DELRULE messages
ip rule add from 10.0.0.1 iif enp3s0f1 oif lo table default
ip rule del from 10.0.0.1 iif enp3s0f1 oif lo table default
