#ifndef STORE_H
#define STORE_H

#include <map>
#include <list>

#include "../../shared/RPC_FLock.hpp"

using namespace std;

class State {
    public:
        typedef map<int, FILE_LOCK> Locks;                  // nr żądania -> blokada
    private:
        Locks _current_locks;
        Locks _waiting_locks;
        
        bool can_lock(FILE_LOCK &lock);
    public:
        bool add_lock(FILE_LOCK &lock);
        void add_waiting_lock(FILE_LOCK &lock);
        bool remove_lock(int request);
        void remove_waiting_lock(int request);
        
        map<int, string> set_up_waiting_locks();            // nr żądania -> host
        bool lock_set_up(int request);

        Locks const& get_locks() const;
};

#endif
