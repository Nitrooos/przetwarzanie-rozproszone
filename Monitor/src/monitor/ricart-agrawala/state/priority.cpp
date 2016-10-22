#include "priority.hpp"

Monitor::Priority::Priority(int req_lamport_time, int tid) : _request_lamport_time(req_lamport_time), _tid(tid) { }

bool Monitor::Priority::lower_than(Message::Base const& msg) const {
    return ( this->_request_lamport_time  > msg.lamport_get_time() ||
            (this->_request_lamport_time == msg.lamport_get_time() && this->_tid > msg.tid()));
}

int Monitor::Priority::get_request_lamport_time() const {
    return this->_request_lamport_time;
}
