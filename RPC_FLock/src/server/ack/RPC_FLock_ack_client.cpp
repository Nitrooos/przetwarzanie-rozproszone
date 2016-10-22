#include "../../shared/RPC_FLock_ack.hpp"

#include "ACK.hpp"

void ACK::notify_client_job_done(pair<int, string> const& job) {
    CLIENT *clnt;
    void  *result_1;
    int  notify_job_done_1_arg = job.first;

    clnt = clnt_create (job.second.c_str(), RPC_FLOCK_CLIENT, notify_job_done_1_arg, "udp");
    if (clnt == NULL) {
        clnt_pcreateerror (job.second.c_str());
        exit (1);
    }

    result_1 = notify_job_done_1(&notify_job_done_1_arg, clnt);
    if (result_1 == (void *) NULL) {
        clnt_perror (clnt, "call failed");
    }

    clnt_destroy (clnt);
}
