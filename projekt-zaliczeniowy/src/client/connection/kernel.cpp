#include "kernel.hpp"
#include "../exceptions/types.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

Connection::Kernel::Kernel() {
    if ((this->_socket = socket(PF_NETLINK, SOCK_RAW, NETLINK_ROUTE)) < 0) {
        throw Exception::Error("Connection::Kernel socket(): cannot create socket");
    }
    
    struct sockaddr_nl sock_addr = this->prepare_sockaddr();
    
    if (bind(this->_socket, (struct sockaddr*) &sock_addr, sizeof sock_addr) < 0) {
        throw Exception::Error("Connection::Kernel bind(): there's no connection between process and kernel");
    }
    
    
}

struct sockaddr_nl Connection::Kernel::prepare_sockaddr() {
    struct sockaddr_nl sock_addr;

    memset(&sock_addr, 0, sizeof sock_addr);
    sock_addr.nl_family = AF_NETLINK;
    sock_addr.nl_pid = getpid();
    sock_addr.nl_groups = RTMGRP_IPV4_ROUTE;
    
    return sock_addr;
}
