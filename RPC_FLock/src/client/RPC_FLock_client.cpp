#include "../shared/RPC_FLock.hpp"
#include "../shared/lock/types.hpp"
#include "../shared/lock/operations.hpp"
#include "arguments/parse.hpp"
#include "ack/ACK.hpp"

#include <iostream>
#include <unistd.h>
#include <iomanip>

void rpc_semaphore_1(Arguments &args) {
    CLIENT *clnt;
    int *result_1;
    char *get_request_id_1_arg;
    LOCK_RESULT *result_2;
    FILE_LOCK lock_file_1_arg;
    bool_t *result_3;
    int unlock_file_1_arg;

    clnt = clnt_create (args._server.c_str(), RPC_SEMAPHORE, DEFAULT_SIGNUM, "udp");
    if (clnt == NULL) {
        clnt_pcreateerror(args._server.c_str());
        exit (1);
    }

    struct timeval timeout = { 25, 0 },
                   retry   = {  5, 0 };
    clnt_control(clnt, CLSET_TIMEOUT, (char *)&timeout);
    clnt_control(clnt, CLSET_RETRY_TIMEOUT, (char *)&retry);

    result_1 = get_request_id_1((void*)&get_request_id_1_arg, clnt);
    if (result_1 == (int *) NULL) {
        clnt_perror (clnt, "call failed");
    }

    strcpy(lock_file_1_arg._filename, args._filename.c_str());
    lock_file_1_arg._start = args._start;
    lock_file_1_arg._request_no = *result_1;
    lock_file_1_arg._length = args._length;
    lock_file_1_arg._type = args._type;
    result_2 = lock_file_1(&lock_file_1_arg, clnt);
    if (result_2 == (void *) NULL) {
        clnt_perror (clnt, "call failed");
    }
    
    if (result_2->_status == Lock::Status::Unavailable) {
        ACK::wait_for(*result_1);
    }

    cout << "Lock: (" << setw(4) << args._start << ", " << setw(4) << args._length << ") " << setw(30) << left;
    switch (result_2->_status) {
        case Lock::Status::OK:          cout << "Lock::Status::OK";          break;
        case Lock::Status::Invalid:     cout << "Lock::Status::Invalid";     break;
        case Lock::Status::NoSuchFile:  cout << "Lock::Status::NoSuchFile";  break;
        case Lock::Status::Unavailable: cout << "Lock::Status::Unavailable"; break;
    }

    sleep(5);

    unlock_file_1_arg = *result_1;
    result_3 = unlock_file_1(&unlock_file_1_arg, clnt);
    if (result_3 == (void *) NULL) {
        clnt_perror (clnt, "call failed");
    }
    
    cout << " Unlock: " << *result_3 << endl;
    
    clnt_destroy (clnt);
}

int main (int argc, char *argv[]) {
    try {
        auto args = Arguments::parse(argc, argv);
        rpc_semaphore_1(args);
        return 0;
    } catch (const char *e) {
        cerr << e << "\n";
    } catch (invalid_argument const& e) {
        cerr << e.what() << "\n";
    } catch (out_of_range const& e) {
        cerr << e.what() << "\n";
    } catch (...) {
        cerr << "Unknown exception\n";
    }
    return 1;
}
