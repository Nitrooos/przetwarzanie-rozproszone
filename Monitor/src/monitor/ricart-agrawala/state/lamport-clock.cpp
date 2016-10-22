#include "lamport-clock.hpp"

#include <algorithm>

Monitor::LamportClock::LamportClock() : _lamport_time(0) { }

int Monitor::LamportClock::get_value() const {
    return this->_lamport_time;
}

int Monitor::LamportClock::tick() {
    return ++this->_lamport_time;
}

void Monitor::LamportClock::synchronize(Message::Base const& msg) {
    this->_lamport_time = std::max(this->_lamport_time, msg.lamport_get_time()) + 1;
}
