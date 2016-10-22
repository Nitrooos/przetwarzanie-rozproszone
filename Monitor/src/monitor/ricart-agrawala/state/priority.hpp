#ifndef PRIORITY_H
#define PRIORITY_H

#include "../message.hpp"

namespace Monitor {
    
    class Priority {
        int _request_lamport_time, _tid;
        
        public:
            Priority(int req_lamport_time, int tid);              // priorytetem jest para (zegar_Lamporta, tid procesu)
            
            bool lower_than(Message::Base const& msg) const;      // sprawdza czy priorytet jest niższy niż procesu, od którego przyszła wiadomość
            int get_request_lamport_time() const;                 // pobierz wartość zegara Lamporta w momencie zażądania dostępu do sekcji krytycznej
    };
    
}

#endif /* PRIORITY_H */
