#ifndef ENTITIES_H
#define ENTITIES_H

#include "monitor.hpp"

class Entity {
    protected:
        ProducerConsumerMonitor &_monitor;
    public:
        Entity(ProducerConsumerMonitor &monitor);
        virtual void do_your_job() = 0;
        static Entity* create(ProducerConsumerMonitor &monitor);        // tworzy Producenta lub Konsumenta w zależności od tid procesu
};

class Producer : public Entity {
    public:
        Producer(ProducerConsumerMonitor &monitor);
        void do_your_job();
};

class Consumer : public Entity {
    public:
        Consumer(ProducerConsumerMonitor &monitor);
        void do_your_job();
};

#endif /* ENTITIES_H */
