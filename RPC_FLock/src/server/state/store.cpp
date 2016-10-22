#include "store.hpp"
#include "../../shared/lock/operations.hpp"

bool State::add_lock(FILE_LOCK &lock) {
    if (this->can_lock(lock)) {
        this->_current_locks[lock._request_no] = lock;
        return true;
    }
    return false;
}

void State::add_waiting_lock(FILE_LOCK &lock) {
    this->_waiting_locks[lock._request_no] = lock;
}

bool State::remove_lock(int request) {
    if (this->lock_set_up(request)) {
        this->_current_locks.erase(request);
        return true;
    }
    return false;
}

void State::remove_waiting_lock(int request) {
    if (this->_waiting_locks.find(request) != this->_waiting_locks.end()) {
        this->_waiting_locks.erase(request);
    }
}

bool State::lock_set_up(int request) {
    return (this->_current_locks.find(request) != this->_current_locks.end());
}

map<int, string> State::set_up_waiting_locks() {
    map<int, string> setup_locks;
    for (auto &l : this->_waiting_locks) {
        if (this->add_lock(l.second)) {
            setup_locks[l.first] = "localhost";
        }
    }
    
    for (auto &l : setup_locks) {
        this->remove_waiting_lock(l.first);
    }
    
    return setup_locks;
}

State::Locks const& State::get_locks() const {
    return this->_current_locks;
}

/*
 * Funkcje prywatne
 */

bool State::can_lock(FILE_LOCK &lock) {
    for (auto &l : this->_current_locks) {
        if (Lock::blocking(lock, l.second)) {
            return false;
        }
    }
    return true;
}
