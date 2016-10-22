#ifndef _RPC_FLOCK_H_RPCGEN
#define _RPC_FLOCK_H_RPCGEN

#include <rpc/rpc.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_NAME_LENGTH 255

struct FILE_LOCK {
    char _filename[MAX_NAME_LENGTH];
    int _request_no;
    int _type;
    int _start;
    int _length;
};
typedef struct FILE_LOCK FILE_LOCK;

struct LOCK_RESULT {
    int _status;
    FILE_LOCK _operation;
};
typedef struct LOCK_RESULT LOCK_RESULT;

#define RPC_SEMAPHORE 0x20000001
#define DEFAULT_SIGNUM 1

#define get_request_id 1
extern  int * get_request_id_1(void *, CLIENT *);
extern  int * get_request_id_1_svc(void *, struct svc_req *);
#define lock_file 2
extern  LOCK_RESULT * lock_file_1(FILE_LOCK *, CLIENT *);
extern  LOCK_RESULT * lock_file_1_svc(FILE_LOCK *, struct svc_req *);
#define unlock_file 3
extern  bool_t * unlock_file_1(int *, CLIENT *);
extern  bool_t * unlock_file_1_svc(int *, struct svc_req *);
extern int rpc_semaphore_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

/* the xdr functions */
extern  bool_t xdr_FILE_LOCK (XDR *, FILE_LOCK*);
extern  bool_t xdr_LOCK_RESULT (XDR *, LOCK_RESULT*);

#ifdef __cplusplus
}
#endif

#endif /* !_RPC_FLOCK_H_RPCGEN */
