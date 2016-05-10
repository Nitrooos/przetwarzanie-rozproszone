#ifndef TCP_ADDRESS_H
#define TCP_ADDRESS_H

struct TCPAddress {
    struct in_addr _ip_address;
    int _port;
};

#endif
