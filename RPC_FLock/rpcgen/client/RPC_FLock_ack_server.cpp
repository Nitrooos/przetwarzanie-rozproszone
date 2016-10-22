#include "RPC_FLock_ack.hpp"

void *notify_job_done_1_svc(int *argp, struct svc_req *rqstp) {
    static char * result;

    /*
     * insert server code here
     */

    return (void *) &result;
}
