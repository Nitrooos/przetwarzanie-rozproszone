#include "algorithm.hpp"

#include <iostream>
#include <iomanip>

Monitor::RicartAgrawala::RicartAgrawala(Connector *connector) : _connector(connector), _accessing_critical_section(false) {
    this->_priority.reset(new Priority(0, connector->system_info()._mpi_tid));
}

void Monitor::RicartAgrawala::lock() {
    unique_lock<mutex> lock(this->_sync._mutex);

    // ubiegam się o dostęp do sekcji krytycznej
    this->_accessing_critical_section = true;
    
    // zapamiętaj wartość zegara Lamporta w momencie zażądania dostępu do sekcji krytycznej
    this->_priority.reset(new Priority(this->_clock.tick(), this->_connector->system_info()._mpi_tid));

    // wyślij do wszystkich żądanie dostępu do sekcji krytycznej
    this->_connector->broadcast(Message::Request(this->_connector->system_info()._mpi_tid, this->_priority->get_request_lamport_time()));

    // czekaj na potwierdzenia od wszystkich
    this->wait_for_all_agrees(lock);
}

void Monitor::RicartAgrawala::unlock() {
    unique_lock<mutex> lock(this->_sync._mutex);

    // wyślij AGREE do wszystkich monitorów czekających
    this->send_agrees_to_waiting();
    
    // już nie ubiegam się o dostęp do sekcji
    this->_accessing_critical_section = false;
}

void Monitor::RicartAgrawala::wait_for_all_agrees(unique_lock<mutex> &lock) {
    while (!this->_sync._ready) {
        this->_sync._cond_variable.wait(lock);
    }
    
    this->_received_agrees.clear();
    this->_sync._ready = false;
    // w tym momencie już wiem, że otrzymałem wszystkie potwierdzenia i mogę wejść do sekcji krytycznej    
}

void Monitor::RicartAgrawala::send_agrees_to_waiting() {
    // wyślij zgody do procesów znajdujących się w kolejce
    for (auto &tid : this->_waiting_processes) {
        this->send_agree(tid);
    }
    this->_waiting_processes.clear();
}

void Monitor::RicartAgrawala::send_agree(int tid) {
    Message::Agree answer(this->_connector->system_info()._mpi_tid, this->_clock.tick());
    this->_connector->send(answer, tid);
}

void Monitor::RicartAgrawala::handle_request(Message::Request const& msg) {
    unique_lock<mutex> lock(this->_sync._mutex);
    this->_clock.synchronize(msg);

    // wyślij zgodę <=> nie starasz się o dostęp do sekcji lub masz mniejszy priorytet (zegar Lamporta)
    if (!this->_accessing_critical_section || this->_priority->lower_than(msg)) {
        this->send_agree(msg.tid());
    } else {
        // w przec. wyp. dodaj do kolejki oczekujących
        this->_waiting_processes.push_back(msg.tid());
    }
}

void Monitor::RicartAgrawala::handle_agree(Message::Agree const& msg) {
    unique_lock<mutex> lock(this->_sync._mutex);
    this->_clock.synchronize(msg);
    
    this->_received_agrees.insert(msg.tid());
    this->_sync._ready = this->has_all_needed_agrees();
    if (this->_sync._ready) {
        // obudź proces tylko jeśli uzbierał wszystkie pozwolenia
        this->_sync._cond_variable.notify_one();
    }
}

bool Monitor::RicartAgrawala::has_all_needed_agrees() const {
    auto info = this->_connector->system_info();
    
    for (int i = 0; i < info._monitors_count; ++i) {
        // jeśli nie mam zgody od jakiegoś innego procesu
        if (i != info._mpi_tid && this->_received_agrees.find(i) == this->_received_agrees.end()) {
            return false;
        }
    }
    // jeśli mam wszystkie potrzebne zgody
    return true;
}

void Monitor::RicartAgrawala::log(string msg) {
    unique_lock<mutex> lock(this->_sync._mutex);
    cout << "[" << setw(3) << this->_connector->system_info()._mpi_tid << "] " << msg;
}
