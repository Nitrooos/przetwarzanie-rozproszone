#ifndef LAMPORT_CLOCK_H
#define LAMPORT_CLOCK_H

#include "../message.hpp"

namespace Monitor {
    
    class LamportClock {
        int _lamport_time;
        
        public:
            LamportClock();
            
            int tick();                                     // zwiększ wartość zegara o 1
            int get_value() const;                          // pobierz aktualny czas Lamporta
            void synchronize(Message::Base const& msg);     // synchronizuj lokalny zegar z wiadomością
    };
    
}

#endif /* LAMPORT_CLOCK_H */
