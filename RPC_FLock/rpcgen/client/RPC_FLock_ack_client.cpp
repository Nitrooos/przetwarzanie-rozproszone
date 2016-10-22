#include "RPC_FLock_ack.hpp"

void rpc_flock_client_1(char *host) {
    CLIENT *clnt;
    void  *result_1;
    int  notify_job_done_1_arg;

    clnt = clnt_create (host, RPC_FLOCK_CLIENT, DEFAULT_SIGNUM, "udp");
    if (clnt == NULL) {
        clnt_pcreateerror (host);
        exit (1);
    }

    result_1 = notify_job_done_1(&notify_job_done_1_arg, clnt);
    if (result_1 == (void *) NULL) {
        clnt_perror (clnt, "call failed");
    }

    clnt_destroy (clnt);
}

int main (int argc, char *argv[]) {
    char *host;

    if (argc < 2) {
        printf ("usage: %s server_host\n", argv[0]);
        exit (1);
    }

    host = argv[1];
    rpc_flock_client_1 (host);
    exit (0);
}
