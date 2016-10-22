#include "mpi-connector.hpp"
#include "../exceptions.hpp"

#include <mpi.h>

Monitor::MPI_Connector::MPI_Connector() {
    this->mpi_init();
}

Monitor::MPI_Connector::~MPI_Connector() {
    int finalized;

    MPI_Finalized(&finalized);
    if (!finalized) {
        MPI_Finalize();
    }
}

void Monitor::MPI_Connector::mpi_init() {
    int initialized;
    
    MPI_Initialized(&initialized);
    if (initialized) {
        throw MPIALreadyInitialized();
    }
    
    int provided_concurrency_level;
    MPI_Init_thread(NULL, NULL, MPI_THREAD_MULTIPLE, &provided_concurrency_level);
    if (provided_concurrency_level != MPI_THREAD_MULTIPLE) {
        throw NotSufficientConcurrencyLevel();
    }
    
    MPI_Comm_size(MPI_COMM_WORLD, &this->_info._monitors_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &this->_info._mpi_tid);
    
    int result_len;
    MPI_Get_processor_name(this->_info._host_name, &result_len);    
}

SystemInfo const& Monitor::MPI_Connector::system_info() const {
    return this->_info;
}

void Monitor::MPI_Connector::send(Message::Base const& msg, int tid) {
    MPI_Send(msg.get_MPI_message().data, Message::Base::Field::COUNT, MPI_INT, tid, msg.type(), MPI_COMM_WORLD);
}

void Monitor::MPI_Connector::send_raw(char *buffer, int length, int tid, int tag) {
    MPI_Send(buffer, length, MPI_CHAR, tid, tag, MPI_COMM_WORLD);
}

void Monitor::MPI_Connector::broadcast(Message::Base const& msg) {
    for (int i = 0; i < this->_info._monitors_count; ++i) {
        if (i != this->_info._mpi_tid) {
            this->send(msg, i);
        }
    }
}

void Monitor::MPI_Connector::broadcast_raw(char *buffer, int length, int tag) {
    for (int i = 0; i < this->_info._monitors_count; ++i) {
        if (i != this->_info._mpi_tid) {
            this->send_raw(buffer, length, i, tag);
        }
    }
}

Monitor::Message::Base* Monitor::MPI_Connector::receive() {
    char *buf;
    MPI_Status status;
    MPI_Message msg;
    MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    
    // wiadomość z zserializowanymi danymi do update'u stanu traktuj inaczej
    if (status.MPI_TAG == Message::Base::Type::UPDATE_SHARED_STATE) {
        int char_amount;
        MPI_Get_count(&status, MPI_CHAR, &char_amount);
        buf = new char[char_amount];
        MPI_Recv(buf, char_amount, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        
        Message::Base *message = new Message::UpdateSharedState(status.MPI_SOURCE, char_amount, buf);
        delete buf;
        return message;
    } else {
        MPI_Recv(&msg, Message::Base::Field::COUNT, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        return Message::Base::build(msg);
    }
}
