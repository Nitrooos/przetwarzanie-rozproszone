#ifndef TCP_H
#define TCP_H

#include <string>

#include "address.hpp"

using namespace std;

namespace Connection {

    class TCP {
        int _socket;
        
        struct sockaddr_in prepare_sockaddr(TCPAddress const& address);
        public:
            TCP(TCPAddress const& address);
            ~TCP();
            void send(string const& data);
    };
    
}

#endif
