#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "../system-info.hpp"
#include "../ricart-agrawala/message.hpp"

namespace Monitor {
    
    class Connector {
        public:
            virtual SystemInfo const& system_info() const = 0;                     // pobierz informacje o systemie wykonawczym
            virtual void send(Message::Base const& msg, int tid) = 0;              // wyślij wiadomość
            virtual void send_raw(char *buffer, int length, int tid, int tag) = 0; // wyślij length bajtów z bufora buffer do procesu tid znacząc tagiem tag
            virtual void broadcast(Message::Base const& msg) = 0;                  // roześlij wiadomość do wszystkich, oprócz siebie
            virtual void broadcast_raw(char *buffer, int length, int tag) = 0;     // roześlij wiadomość z bufora o długości length do wszystkich, oprócz siebie
            virtual Message::Base* receive() = 0;                                  // czekaj na jakąkolwiek wiadomość przychodzącą
    };
    
}

#endif /* CONNECTOR_H */
