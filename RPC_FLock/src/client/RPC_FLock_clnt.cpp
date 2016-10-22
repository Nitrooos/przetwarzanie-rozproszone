#include <memory.h>
#include "../shared/RPC_FLock.hpp"

/* Ten timeout będzie ignorowany, ponieważ wcześniej ustawiłem właściwy przez clnt_control */
static struct timeval TIMEOUT = { 0, 0 };

int *get_request_id_1(void *argp, CLIENT *clnt) {
    static int clnt_res;

    memset((char *)&clnt_res, 0, sizeof(clnt_res));
    if (clnt_call (clnt, get_request_id,
        (xdrproc_t) xdr_void, (caddr_t) argp,
        (xdrproc_t) xdr_int, (caddr_t) &clnt_res,
        TIMEOUT) != RPC_SUCCESS) {
        return (NULL);
    }
    return (&clnt_res);
}

LOCK_RESULT *lock_file_1(FILE_LOCK *argp, CLIENT *clnt) {
    static char clnt_res;

    memset((char *)&clnt_res, 0, sizeof(clnt_res));
    if (clnt_call (clnt, lock_file,
        (xdrproc_t) xdr_FILE_LOCK, (caddr_t) argp,
        (xdrproc_t) xdr_LOCK_RESULT, (caddr_t) &clnt_res,
        TIMEOUT) != RPC_SUCCESS) {
        return (NULL);
    }
    return ((LOCK_RESULT *)&clnt_res);
}

bool_t *unlock_file_1(int *argp, CLIENT *clnt) {
    static char clnt_res;

    memset((char *)&clnt_res, 0, sizeof(clnt_res));
    if (clnt_call (clnt, unlock_file,
        (xdrproc_t) xdr_int, (caddr_t) argp,
        (xdrproc_t) xdr_bool, (caddr_t) &clnt_res,
        TIMEOUT) != RPC_SUCCESS) {
        return (NULL);
    }
    return ((bool_t *)&clnt_res);
}
