#ifndef KERNEL_H
#define KERNEL_H

#include <linux/rtnetlink.h>

using namespace std;

namespace Connection {

    class Kernel {
        int _socket;
        
        struct sockaddr_nl prepare_sockaddr();
        int receive_msg_headers(char *buf);
        public:
            Kernel();
            void receive();
    };
    
}

#endif
