#ifndef MONITOR_BASE_H
#define MONITOR_BASE_H

#include "state.hpp"
#include "mpi/connector.hpp"

#include <thread>
#include <vector>
#include <string>
#include <memory>

using namespace std;

namespace Monitor {

    class Base {
        protected:
            unique_ptr<State> _state;                           // stan monitora
            MPI_Connector _connector;                           // używany do komunikacji z innymi monitorami
            vector<thread> _threads;                            // wątek odbierający komunikaty i odpowiadający na bieżąco

            void start_threads();                               // start wykonania wątków odbierającego i przetwarzającego
            void receiving_task();                              // wątek odpowiadający na żądania innych monitorów
        public:
            Base(SharedVariables *s);                           // wskaźnik na własną strukturę ze współdzielonymi zmiennymi
            
            void lock();                                        // uzyskaj dostęp do rozproszonej sekcji krytycznej
            void unlock();                                      // zwolnij dostęp do rozproszonej sekcji krytycznej

            int get_tid() const;                                // pobierz tid procesu monitora
            void log(string msg) const;                         // wypisuje na ekran wiadomość diagnostczną
            
            /* tradycyjne API monitora */
            void wait(int conditional);                         // czekaj na zmiennej warunkowej identyfikowanej przez conditional
            void pulse(int conditional);                        // obudź któryś z procesów czekających na zm. war. conditional
            void pulse_all();                                   // obudź wszystkie czekające procesy
    };

}

#endif /* MONITOR_BASE_H */
