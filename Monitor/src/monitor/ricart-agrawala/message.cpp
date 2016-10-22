#include "message.hpp"
#include "../state.hpp"

#include <cstring>

Monitor::Message::Base::Base(Type type, int tid, int lamport) {
    this->_message.data[Field::TYPE]    = type;
    this->_message.data[Field::TID]     = tid;
    this->_message.data[Field::LAMPORT] = lamport;
}

Monitor::MPI_Message Monitor::Message::Base::get_MPI_message() const {
    return this->_message;
}

int Monitor::Message::Base::type() const {
    return this->_message.data[Field::TYPE];
}

int Monitor::Message::Base::tid() const {
    return this->_message.data[Field::TID];
}

int Monitor::Message::Base::lamport_get_time() const {
    return this->_message.data[Field::LAMPORT];
}

Monitor::Message::Request::Request(int tid, int lamport) : Base(Type::REQUEST, tid, lamport) { }

void Monitor::Message::Request::handle(Monitor::State *monitor_state) {
    monitor_state->algorithm()->handle_request(*this);
}

Monitor::Message::Agree::Agree(int tid, int lamport) : Base(Type::AGREE, tid, lamport) { }

void Monitor::Message::Agree::handle(Monitor::State *monitor_state) {
    monitor_state->algorithm()->handle_agree(*this);
}

Monitor::Message::UpdateSharedState::UpdateSharedState(int tid, int length, char *buf)
    : Base(Type::UPDATE_SHARED_STATE, tid, length), _encoded_message(new char[length])
{
    memcpy(this->_encoded_message, buf, length);                        // kopiuj bajt po bajcie do bufora
}

Monitor::Message::UpdateSharedState::~UpdateSharedState() {
    delete [] this->_encoded_message;
}

char *Monitor::Message::UpdateSharedState::get_encoded_state() const {
    return this->_encoded_message;
}

void Monitor::Message::UpdateSharedState::handle(Monitor::State *monitor_state) {
    monitor_state->handle_update_shared_state(*this);
}

Monitor::Message::UpdateSharedStateACK::UpdateSharedStateACK(int tid) : Base(Type::UPDATE_SHARED_STATE_ACK, tid, -1) { }

void Monitor::Message::UpdateSharedStateACK::handle(Monitor::State *monitor_state) {
    monitor_state->handle_update_shared_state_ACK(*this);
}

Monitor::Message::Pulse::Pulse() : Base(Type::PULSE, -1, -1) { }

void Monitor::Message::Pulse::handle(Monitor::State *monitor_state) {
    monitor_state->handle_pulse(*this);
}

Monitor::Message::AreYouWaiting::AreYouWaiting(int tid) : Base(Type::ARE_YOU_WAITING, tid, -1) { }

void Monitor::Message::AreYouWaiting::handle(Monitor::State *monitor_state) {
    monitor_state->handle_are_you_waiting(*this);
}

Monitor::Message::AreYouWaitingResponse::AreYouWaitingResponse(int tid, int waiting)
    : Base(Type::ARE_YOU_WAITING_RESPONSE, tid, waiting) { }

void Monitor::Message::AreYouWaitingResponse::handle(Monitor::State *monitor_state) {
    monitor_state->handle_are_you_waiting_response(*this);
}

int Monitor::Message::AreYouWaitingResponse::waiting() const {
    return this->_message.data[Field::WAITING];
}

Monitor::Message::Base *Monitor::Message::Base::build(MPI_Message &msg) {
    switch (msg.data[Field::TYPE]) {
        case Type::REQUEST:
            return new Request(msg.data[Field::TID], msg.data[Field::LAMPORT]);
        case Type::AGREE:
            return new Agree(msg.data[Field::TID], msg.data[Field::LAMPORT]);
        case Type::UPDATE_SHARED_STATE_ACK:
            return new UpdateSharedStateACK(msg.data[Field::TID]);
        case Type::ARE_YOU_WAITING:
            return new AreYouWaiting(msg.data[Field::TID]);
        case Type::ARE_YOU_WAITING_RESPONSE:
            return new AreYouWaitingResponse(msg.data[Field::TID], msg.data[Field::WAITING]);
        case Type::PULSE:
            return new Pulse();
    }
}
