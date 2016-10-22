#ifndef ACK_H
#define ACK_H

#include <pthread.h>

namespace ACK {
    struct Server {
        int _waiting_ACK_request_no;
        pthread_t _thread;
        pthread_mutex_t _mutex;
        pthread_cond_t _received_ACK;
    };
    
    void wait_for(int request_no);
    Server *get_server();
}

#endif
