#include "kernel.hpp"
#include "../exceptions/types.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <iostream>

Connection::Kernel::Kernel() {
    if ((this->_socket = socket(PF_NETLINK, SOCK_RAW, NETLINK_ROUTE)) < 0) {
        throw Exception::Error("Connection::Kernel socket(): cannot create socket");
    }
    
    struct sockaddr_nl sock_addr = this->prepare_sockaddr();
    
    if (bind(this->_socket, (struct sockaddr*) &sock_addr, sizeof sock_addr) < 0) {
        throw Exception::Error("Connection::Kernel bind(): there's no connection between process and kernel");
    }
}

Connection::Kernel::~Kernel() {
    close(this->_socket);
}

struct sockaddr_nl Connection::Kernel::prepare_sockaddr() {
    struct sockaddr_nl sock_addr;

    memset(&sock_addr, 0, sizeof sock_addr);
    sock_addr.nl_family = AF_NETLINK;
    sock_addr.nl_pid = getpid();
    sock_addr.nl_groups = RTMGRP_IPV4_ROUTE  |
                          RTMGRP_IPV4_RULE   |
                          RTMGRP_LINK        |
                          RTMGRP_IPV4_IFADDR |
                          RTMGRP_NEIGH       |
                          RTMGRP_NOTIFY;
    
    return sock_addr;
}

list<Message::Base*> Connection::Kernel::receive() {
    list<Message::Base*> messages;
    
    int nl_length;
    char buf[8192];
    struct nlmsghdr *nl_message;
    struct rtmsg *rtp;
    
    nl_length = this->receive_msg_headers(buf, sizeof buf);
    nl_message = (struct nlmsghdr*) buf;
    for(;NLMSG_OK(nl_message, nl_length); nl_message = NLMSG_NEXT(nl_message, nl_length)) {
        // spróbuj zbudować nową, poprawną wiadomość
        try {
            auto new_msg = Message::Base::build(nl_message);
            // jeśli się udało, to dodaj do listy
            if (new_msg != nullptr) {
                messages.push_back(new_msg);
            }
        } catch (Exception::Warning const& e) {
            cerr << e.what();
        }
    }
    
    return messages;
}

int Connection::Kernel::receive_msg_headers(char *buf, int buf_len) {
    char *ptr;
    int length;
    struct nlmsghdr *nl_msg_header;
    
    ptr = buf;
    length = 0;
    
    /// ten blok ma na celu ustalenie wartości nllen 
    do {
        /// czekanie na wiadomość
        int received_bytes = recv(this->_socket, ptr, buf_len - length, 0);
        /// pobranie headera wiadomości
        nl_msg_header = (struct nlmsghdr*) ptr;
        ptr += received_bytes;
        length += received_bytes;
        /// aż do odebrania wszystkich nagłówków
    } while (nl_msg_header->nlmsg_type == NLMSG_DONE);
    
    return length;
}
