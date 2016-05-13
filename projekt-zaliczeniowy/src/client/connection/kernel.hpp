#ifndef KERNEL_H
#define KERNEL_H

#include <linux/rtnetlink.h>
#include <list>

#include "../message/base.hpp"

using namespace std;

namespace Connection {

    class Kernel {
        int _socket;
        
        struct sockaddr_nl prepare_sockaddr();
        int receive_msg_headers(char *buf, int buf_len);
        public:
            Kernel();
            list<Message::Base*> receive();
    };
    
}

#endif
