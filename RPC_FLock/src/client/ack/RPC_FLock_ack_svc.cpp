#include "../../shared/RPC_FLock_ack.hpp"
#include "ACK.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

ACK::Server global_server;

static void rpc_flock_client_1(struct svc_req *rqstp, register SVCXPRT *transp) {
    union {
        int notify_job_done_1_arg;
    } argument;
    char *result;
    xdrproc_t _xdr_argument, _xdr_result;
    char *(*local)(char *, struct svc_req *);

    switch (rqstp->rq_proc) {
    case NULLPROC:
        (void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
        return;

    case notify_job_done:
        _xdr_argument = (xdrproc_t) xdr_int;
        _xdr_result = (xdrproc_t) xdr_void;
        local = (char *(*)(char *, struct svc_req *)) notify_job_done_1_svc;
        break;

    default:
        svcerr_noproc (transp);
        return;
    }
    memset ((char *)&argument, 0, sizeof (argument));
    if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
        svcerr_decode (transp);
        return;
    }
    result = (*local)((char *)&argument, rqstp);
    if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
        svcerr_systemerr (transp);
    }
    if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
        fprintf (stderr, "%s", "unable to free arguments");
        exit (1);
    }
    return;
}

void *server_task(void *args) {
    register SVCXPRT *transp;

    pmap_unset (RPC_FLOCK_CLIENT, global_server._waiting_ACK_request_no);

    transp = svcudp_create(RPC_ANYSOCK);
    if (transp == NULL) {
        fprintf (stderr, "%s", "cannot create udp service.");
        pthread_exit(NULL);
    }
    if (!svc_register(transp, RPC_FLOCK_CLIENT, global_server._waiting_ACK_request_no, rpc_flock_client_1, IPPROTO_UDP)) {
        fprintf (stderr, "%s", "unable to register (RPC_FLOCK_CLIENT, global_server._waiting_ACK_request_no, udp).");
        pthread_exit(NULL);
    }

    svc_run ();
}

void ACK::wait_for(int request_no) {
    global_server._waiting_ACK_request_no = request_no;

    if (pthread_create(&global_server._thread, NULL, server_task, NULL)) {
        throw "Nie udało się utworzyć wątku serwera czekającego na potwierdzenia RPC";
    }

    pthread_cond_wait(&global_server._received_ACK, &global_server._mutex);
}

ACK::Server *ACK::get_server() {
    return &global_server;
}
