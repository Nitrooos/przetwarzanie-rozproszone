#ifndef MONITOR_H
#define MONITOR_H

#include <monitor/base.hpp>

#define BUFFER_SIZE 10

// zmienna współdzielona między monitory
struct SharedBuffer : public Monitor::SharedVariables {
    int _buffer[BUFFER_SIZE] = { 0 };
    int _products_count{0},                 // bufor początkowo pusty
        _next_product_no{1},                // pierwszy wyprodukowany element będzie miał wartość 1    
        _index_producer{0},                 // pobieranie zacznij od indeksu 0
        _index_consumer{0};                 // produkcję zacznij od indeksu 0
    
    virtual bool transform(XDR *xdr);       // serializacja/deserializacja XDR
    virtual string to_string();             // reprezentacja stanu
    virtual int size_of();
};

class ProducerConsumerMonitor : public Monitor::Base {
    SharedBuffer *_shared_buffer;

    // zmienne warunkowe, na których będzie mógł czekać monitor
    enum CondVariable : int { CAN_PRODUCE = 1, CAN_CONSUME = 2 };
    
    public:
        ProducerConsumerMonitor();
        void produce();
        void consume();
};

#endif /* MONITOR_H */
