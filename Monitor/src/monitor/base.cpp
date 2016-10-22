#include "base.hpp"

Monitor::Base::Base(SharedVariables *s) {
    this->_state.reset(new State(&this->_connector, s));
    this->start_threads();
}

int Monitor::Base::get_tid() const {
    return this->_connector.system_info()._mpi_tid;
}

void Monitor::Base::wait(int conditional) {
    // czekaj na zadanej zmiennej warunkowej
    this->_state->waiting() = conditional;
    
    // zwolnij rozproszoną sekcję krytyczną
    this->_state->algorithm()->unlock();

    // uśpij proces na czekaniu na wiadomość o wybudzeniu
    this->_state->suspend();
    
    this->_state->waiting() = false;
    
    // ponownie wejdź do sekcji krytycznej
    this->_state->algorithm()->lock();
}

void Monitor::Base::pulse(int conditional) {
    // wyślij zapytanie o to, kto aktualnie czeka w wait
    this->_connector.broadcast(Message::AreYouWaiting(this->_connector.system_info()._mpi_tid));

    // uśpij proces na czekaniu na zebranie wszystkich odpowiedzi
    this->_state->suspend();
    
    // wybierz jakąś odpowiedź i wyślij jej PULSE
    this->_state->send_someone_pulse(conditional);
}

void Monitor::Base::pulse_all() {
    // wyślij wiadomość wybudzenia do wszystkich monitorów - i tak zareagują na nią tylko te czekające w wait
    this->_connector.broadcast(Message::Pulse());
}

void Monitor::Base::lock() {
    this->_state->algorithm()->lock();
}

void Monitor::Base::unlock() {
    // wyślij każdemu aktualizację współdzielonego stanu przed opuszczeniem sekcji krytycznej
    this->_state->send_everyone_update();
    
    this->_state->algorithm()->unlock();
}

void Monitor::Base::receiving_task() {
    while (true) {
        Message::Base *msg = this->_connector.receive();    // odbierz wiadomość
        msg->handle(this->_state.get());                    // obsłuż ją    
        delete msg;                                         // posprzątaj
    }
}

void Monitor::Base::start_threads() {
    this->_threads.emplace_back(&Monitor::Base::receiving_task, this);
    this->_threads.back().detach();
}

void Monitor::Base::log(string msg) const {
    this->_state->algorithm()->log(msg);
}
