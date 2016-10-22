#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "../mpi/mpi-connector.hpp"
#include "message.hpp"
#include "state/synchronizer.hpp"
#include "state/priority.hpp"
#include "state/lamport-clock.hpp"

#include <memory>
#include <mutex>
#include <set>
#include <deque>

using namespace std;

namespace Monitor {

    class RicartAgrawala {
            Connector *_connector;                              // odpowiada za inicjalizację i bezpieczne wyłączenie MPI
            unique_ptr<Priority> _priority;                     // priorytet procesu aplikacyjnego (ważny przy ustalaniu dostępu do sekcji krytycznej)
            LamportClock _clock;                                // zegar logiczny Lamporta
            set<int> _received_agrees;                          // zbiór :tid procesów, od których otrzymaliśmy zgodę na wejście do sekcji krytycznej
            deque<int> _waiting_processes;                      // kolejka :tid monitorów czekających na dostęp do sekcji krytycznej
            Synchronizer _sync;                                 // synchronizacja czekania na otrzymanie wszystkich zgód na wejście do sekcji krytycznej
            bool _accessing_critical_section;                   // czy monitor próbuje dostać się do sekcji krytycznej?
            
            void wait_for_all_agrees(unique_lock<mutex> &lock); // czekaj na uzyskanie wszystkich zgód (usypianie procesu)
            void send_agrees_to_waiting();                      // wyślij zgody do czekających na nie monitorów
            void send_agree(int tid);                           // wyślij zgodę do monitora tid
            bool has_all_needed_agrees() const;                 // czy mamy wszystkie wymagane zgody?
        public:
            RicartAgrawala(Connector *connector);
        
            void lock();                                        // uzyskaj dostęp do rozproszonej sekcji krytycznej
            void unlock();                                      // zwolnij dostęp do rozproszonej sekcji krytycznej
            
            void handle_request(Message::Request const& msg);   // obsłuż przychodzące żądanie wejścia do sekcji krytycznej
            void handle_agree  (Message::Agree   const& msg);   // obsłuż przychodzącą zgodę na wejście do sekcji krytycznej
            
            void log(string msg);
    };

}

#endif /* ALGORITHM_H */
