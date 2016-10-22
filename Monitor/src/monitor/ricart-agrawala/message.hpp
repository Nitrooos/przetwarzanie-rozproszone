#ifndef MONITOR_MESSAGE_H
#define MONITOR_MESSAGE_H

namespace Monitor {
    
    class State;

    struct MPI_Message {
        int data[3];
    };

    namespace Message {

        class Base {
            protected:
                MPI_Message _message;
            public:
                enum Field { TYPE = 0, TID = 1, LAMPORT = 2, WAITING = 2, LENGTH = 2, COUNT = WAITING + 1 };
                enum Type {
                    REQUEST, AGREE,                                 // związane z algorytmem Ricarta-Agrawala
                    UPDATE_SHARED_STATE, UPDATE_SHARED_STATE_ACK,   // update współdzielonego stanu i odpowiednie potwierdzenia
                    ARE_YOU_WAITING, ARE_YOU_WAITING_RESPONSE,      // zapytania o czekanie na zmiennej warunkowej i odpowiedzi
                    PULSE,                                          // wybudzanie monitora ze śpiączki
                };

                Base(Type type, int tid, int lamport);
                MPI_Message get_MPI_message() const;
                int type() const;                                   // typ (TAG) wiadomości 
                int tid() const;                                    // tid nadawcy
                int lamport_get_time() const;                       // wartość zegara Lamporta nadawcy
                
                virtual void handle(Monitor::State *state) = 0;
                
                static Base *build(MPI_Message &msg);               // stwórz odpowiednią wiadomość na podstawie MPI_Message
        };
    
        class Request : public Base {
            public:
                Request(int tid, int lamport);
                virtual void handle(Monitor::State *state);
        };
        
        class Agree : public Base {
            public:
                Agree(int tid, int lamport);
                virtual void handle(Monitor::State *state);
        };
        
        class UpdateSharedState : public Base {
            char *_encoded_message;
            
            public:
                UpdateSharedState(int tid, int length, char *buf);
                ~UpdateSharedState();
                virtual void handle(Monitor::State *state);
                
                char *get_encoded_state() const;                    // zserializowana postać danych
        };
        
        class UpdateSharedStateACK : public Base {
            public:
                UpdateSharedStateACK(int tid);
                virtual void handle(Monitor::State *state);
        };
        
        class Pulse : public Base {
            public:
                Pulse();
                virtual void handle(Monitor::State *state);
        };
        
        class AreYouWaiting : public Base {
            public:
                AreYouWaiting(int tid);
                virtual void handle(Monitor::State *state);
        };
        
        class AreYouWaitingResponse : public Base {
            public:
                AreYouWaitingResponse(int tid, int waiting);
                virtual void handle(Monitor::State *state);
                
                int waiting() const;                                // na jakiej zmiennej czeka pytany proces? lub 0 jeśli na żadnej
        };

    }

}

#endif /* MONITOR_MESSAGE_H */
