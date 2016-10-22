#ifndef MPI_CONNECTOR_H
#define MPI_CONNECTOR_H

#include "connector.hpp"

namespace Monitor {
    
    class MPI_Connector : public Connector {
        SystemInfo _info;
        void mpi_init();                                                        // inicjalizowanie MPI i zebranie informacji o systemie
        
        public:
            MPI_Connector();
            ~MPI_Connector();
            
            virtual void send(Message::Base const& msg, int tid);                // wyślij wiadomość
            virtual void send_raw(char *buffer, int length, int tid, int tag);   // wyślij length bajtów z bufora buffer do procesu tid znacząc tagiem tag
            virtual Message::Base* receive();                                    // czekaj na jakąkolwiek wiadomość przychodzącą
            virtual void broadcast(Message::Base const& msg);                    // roześlij wiadomość do wszystkich, oprócz siebie
            virtual void broadcast_raw(char *buffer, int length, int tag);       // roześlij wiadomość z bufora buffer o długości length do wszystkich, oprócz siebie
            virtual SystemInfo const&system_info() const;                        // pobierz informacje o systemie wykonawczym
    };
    
}

#endif /* CONNECTOR_H */
