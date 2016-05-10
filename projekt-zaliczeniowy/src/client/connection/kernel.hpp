#ifndef KERNEL_H
#define KERNEL_H

#include <linux/rtnetlink.h>

namespace Connection {

    class Kernel {
        int _socket;
        
        struct sockaddr_nl prepare_sockaddr();
        public:
            Kernel();
    };
    
}

#endif
