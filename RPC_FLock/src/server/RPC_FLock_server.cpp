#include "../shared/RPC_FLock.hpp"
#include "../shared/lock/operations.hpp"
#include "../shared/file/default.hpp"
#include "file/access.hpp"
#include "state/store.hpp"
#include "ack/ACK.hpp"

#include <string>

using namespace std;

static State state;

int *get_request_id_1_svc(void *argp, struct svc_req *rqstp) {
    static int result = 0;
    return &(++result);
}

LOCK_RESULT *lock_file_1_svc(FILE_LOCK *argp, struct svc_req *rqstp) {
    static LOCK_RESULT result;

    int lock_validity = File::check_lock(argp->_filename, argp->_start, argp->_length);
    if (lock_validity == Lock::Status::OK) {
        if (!state.lock_set_up(argp->_request_no) && state.add_lock(*argp)) {
            File::visualise(File::content(File::DefaultFile), state.get_locks());
            result._status = Lock::Status::OK;
        } else {
            state.add_waiting_lock(*argp);
            result._status = Lock::Status::Unavailable;
        }
    }
    
    Lock::clone(result._operation, *argp);

    return &result;
}

bool_t *unlock_file_1_svc(int *argp, struct svc_req *rqstp) {
    static bool result;

    result = state.remove_lock(*argp);
    
    if (result) {
        map<int, string> setup_locks = state.set_up_waiting_locks();
        for (auto &job : setup_locks) {
            ACK::notify_client_job_done(job);
        }
        
        File::visualise(File::content(File::DefaultFile), state.get_locks());
    }

    return (bool_t *) &result;
}
