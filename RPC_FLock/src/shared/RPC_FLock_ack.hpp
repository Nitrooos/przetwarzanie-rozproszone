#ifndef _RPC_FLOCK_CLIENT_H_RPCGEN
#define _RPC_FLOCK_CLIENT_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif

#define RPC_FLOCK_CLIENT 0x20000002
#define DEFAULT_SIGNUM 1

#define notify_job_done 1
extern  void * notify_job_done_1(int *, CLIENT *);
extern  void * notify_job_done_1_svc(int *, struct svc_req *);
extern int rpc_flock_client_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#ifdef __cplusplus
}
#endif

#endif
