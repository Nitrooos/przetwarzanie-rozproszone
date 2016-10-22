#include "../../shared/RPC_FLock_ack.hpp"
#include "ACK.hpp"

#include <iostream>

void *notify_job_done_1_svc(int *argp, struct svc_req *rqstp) {
    static char * result;

    ACK::Server *s = ACK::get_server();
    if (*argp == s->_waiting_ACK_request_no) {
        pthread_cond_signal(&s->_received_ACK);
    }

    return (void *) &result;
}
