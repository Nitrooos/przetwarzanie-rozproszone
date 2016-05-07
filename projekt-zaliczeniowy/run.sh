#!/bin/bash

debug/PF_NETLINK > out.txt
monitorPID=$!

gksudo ifconfig enp3s0f1:0 10.0.0.1
gksudo ifconfig enp3s0f1:1 10.0.0.2

kill -09 $monitorPID

