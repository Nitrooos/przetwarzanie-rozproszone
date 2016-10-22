#include "../../shared/RPC_FLock_ack.hpp"

#include <memory.h>

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

void *notify_job_done_1(int *argp, CLIENT *clnt) {
    static char clnt_res;

    memset((char *)&clnt_res, 0, sizeof(clnt_res));
    if (clnt_call (clnt, notify_job_done,
        (xdrproc_t) xdr_int, (caddr_t) argp,
        (xdrproc_t) xdr_void, (caddr_t) &clnt_res,
        TIMEOUT) != RPC_SUCCESS) {
        return (NULL);
    }
    return ((void *)&clnt_res);
}
