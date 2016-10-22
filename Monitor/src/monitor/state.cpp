#include "state.hpp"

#include <algorithm>

Monitor::State::State(Connector *connector, SharedVariables *vars) 
    : _algorithm(new RicartAgrawala(connector)), _connector(connector), _waiting(false),
      _shared_vars(vars), _converter(new eXDR::Converter(vars->size_of()))
{ }

int &Monitor::State::waiting() {
    return this->_waiting;
}

void Monitor::State::suspend() {
    // proces będzie czekał na zmiennej warunkowej do wybudzenia
    unique_lock<mutex> lock(this->_sync._mutex);
    while (!this->_sync._ready) {
        this->_sync._cond_variable.wait(lock);
    }
    
    // w tym momencie monitor został wybudzony
    this->_sync._ready = false;
}

void Monitor::State::send_someone_pulse(int conditional) {
    int chosen_tid = this->get_someone_waiting_tid(conditional);
    // jeśli znaleziono odpowiedniego odbiorcę
    if (chosen_tid >= 0) {
        this->_connector->send(Message::Pulse(), chosen_tid);
    }
    this->_are_you_waiting_responses.clear();
}

void Monitor::State::send_everyone_update() {
    char *encoded_data = this->_converter->_encoder->serialize(this->_shared_vars.get());
    this->_connector->broadcast_raw(encoded_data, this->_shared_vars->size_of(), Message::Base::Type::UPDATE_SHARED_STATE);

    // czekaj na otrzymanie wszystkich potwierdzeń o aktualizacji od innych monitorów
    this->suspend();

    // wyczyść mapę odpowiedzi o aktualizacji stanu w innych monitorach
    this->_update_state_responses.clear();
}

void Monitor::State::handle_update_shared_state(Message::UpdateSharedState const& msg) {
    this->_converter->_decoder->deserialize(msg.get_encoded_state(), this->_shared_vars.get());
    this->_connector->send(Message::UpdateSharedStateACK(this->_connector->system_info()._mpi_tid), msg.tid());
}

void Monitor::State::handle_update_shared_state_ACK(Message::UpdateSharedStateACK const& msg) {
    this->handle_response(msg.tid(), true, this->_update_state_responses);
}

void Monitor::State::handle_pulse(Message::Pulse const& msg) {
    if (this->_waiting) {
        unique_lock<mutex> lock(this->_sync._mutex);
        this->_sync._ready = true;
        this->_waiting = false;                                     // już nie czekasz na żadnej zmiennej warunkowej
        this->_sync._cond_variable.notify_one();                    // wybudź wątek monitora
    }
}

void Monitor::State::handle_are_you_waiting(Message::AreYouWaiting const& msg) {
    this->_connector->send(Message::AreYouWaitingResponse(this->_connector->system_info()._mpi_tid, this->_waiting), msg.tid());
}

void Monitor::State::handle_are_you_waiting_response(Message::AreYouWaitingResponse const& msg) {
    this->handle_response(msg.tid(), msg.waiting(), this->_are_you_waiting_responses);
}

unique_ptr<Monitor::RicartAgrawala> &Monitor::State::algorithm() {
    return this->_algorithm;
}

bool Monitor::State::has_all_needed_responses(map<int, int> &responses) const {
    auto info = this->_connector->system_info();
    
    for (int i = 0; i < info._monitors_count; ++i) {
        // jeśli nie mam zgody od jakiegoś innego procesu
        if (i != info._mpi_tid && responses.find(i) == responses.end()) {
            return false;
        }
    }
    // jeśli mam wszystkie potrzebne zgody
    return true;
}

void Monitor::State::handle_response(int from, int response, map<int, int> &responses) {
    unique_lock<mutex> lock(this->_sync._mutex);
    responses[from] = response;
    this->_sync._ready = this->has_all_needed_responses(responses);
    if (this->_sync._ready) {
        this->_sync._cond_variable.notify_one();
    }
}

int Monitor::State::get_someone_waiting_tid(int conditional) const {
    // policz ile monitorów czeka na zm. war. conditional
    int waiting_on_conditional = count_if(this->_are_you_waiting_responses.begin(), this->_are_you_waiting_responses.end(), [&] (pair<int, int> const& r) {
        return r.second == conditional;
    });
    
    // jeśli czeka choć jeden
    if (waiting_on_conditional > 0) {
        // znajdź ich wszystkich
        auto item = find_if(this->_are_you_waiting_responses.begin(), this->_are_you_waiting_responses.end(), [&] (pair<int, int> const& r) {
            return r.second == conditional;
        });
        // wybierz losowo jakiegoś
        std::advance(item, rand() % waiting_on_conditional);
        
        return item->first;
    }
    
    // nikt nie czeka na danej zmiennej warunkowej
    return -1;  
}
