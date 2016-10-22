#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <string>

#include "../RPC_FLock.hpp"

using namespace std;

namespace Lock {
    void clone(FILE_LOCK &to, FILE_LOCK &from);
    bool blocking(FILE_LOCK &l1, FILE_LOCK &l2);
    string visualise(FILE_LOCK &l, const string &text, int start);
    
    namespace Status {
        const int OK = 0, Invalid = 1, NoSuchFile = 2, Unavailable = 3;
    }
}

#endif
