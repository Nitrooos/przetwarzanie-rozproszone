#ifndef TCP_H
#define TCP_H

#include "address.hpp"

class Connection {
    int _socket;
    
    struct sockaddr_in prepare_sockaddr(TCPAddress const& address);
    public:
        Connection(TCPAddress const& address);
        ~Connection();
        void send(char *data);
};

#endif
