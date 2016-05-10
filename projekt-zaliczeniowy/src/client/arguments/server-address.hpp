#ifndef SERVER_ADDRESS_H
#define SERVER_ADDRESS_H

struct ServerAddress {
    struct in_addr _ip_address;
    int _port;
};

#endif /* SERVER-ADDRESS_H */
