#ifndef TCP_H
#define TCP_H

#include "address.hpp"

namespace Connection {

    class TCP {
        int _socket;
        
        struct sockaddr_in prepare_sockaddr(TCPAddress const& address);
        public:
            TCP(TCPAddress const& address);
            ~TCP();
            void send(char *data);
    };
    
}

#endif
