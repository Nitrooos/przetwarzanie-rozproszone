#ifndef STATE_H
#define STATE_H

#include "ricart-agrawala/algorithm.hpp"
#include "xdr/shared-variables.hpp"
#include "xdr/converter.hpp"

#include <map>

using namespace std;

namespace Monitor {
    
    class State {
        protected:
            unique_ptr<RicartAgrawala> _algorithm;              // algorytm Ricarta-Agrawala dostępu do sekcji krytycznej
            unique_ptr<SharedVariables> _shared_vars;           // zmienne współdzielone przez monitory
            unique_ptr<eXDR::Converter> _converter;             // konwerter dane -> XDR oraz XDR -> dane
            Synchronizer _sync;                                 // synchronizacja w oczekiwaniu na wybudzenie monitora za pomocą notify
            Connector *_connector;                              // umożliwia wysyłanie i odbiór wiadomości
            int _waiting;                                       // na jakiej zm. warunkowej czeka monitor? 0 jeśli na żadnej
            map<int, int> _are_you_waiting_responses,           // odpowiedzi innych monitorów czy czekają w waiting (id, czy_czeka)
                          _update_state_responses;              // odpowiedzi innych monitorów o udanej aktualizacji lokalnych stanów
            
            bool has_all_needed_responses(map<int, int> &responses) const;
            void handle_response(int from, int response, map<int, int> &responses);
            int get_someone_waiting_tid(int conditional) const;
        public:
            State(Connector *connector, SharedVariables *vars);
            
            int &waiting();                                     // referencja do zm. warunkowej, na której czeka monitor (lub 0 jeśli nie czeka)
            void suspend();                                     // uśpij monitor (na lokalnej zmiennej warunkowej)
            void send_someone_pulse(int conditional);           // wyślij pulse komukolwiek, kto czeka na zm. war. conditional
            void send_everyone_update();                        // wyślij każdemu update stanu współdzielonego
            unique_ptr<RicartAgrawala> &algorithm();
            
            /* Metody obsługujące różne typy przybyłych wiadomości */
            void handle_update_shared_state(Message::UpdateSharedState const& msg);
            void handle_update_shared_state_ACK(Message::UpdateSharedStateACK const& msg);
            void handle_pulse(Message::Pulse const& msg);
            void handle_are_you_waiting(Message::AreYouWaiting const& msg);
            void handle_are_you_waiting_response(Message::AreYouWaitingResponse const& msg);
    };

}

#endif /* STATE_H */
