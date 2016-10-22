#ifndef APPLICATION_H
#define APPLICATION_H

#include "producer-consumer/monitor.hpp"
#include "producer-consumer/entities.hpp"

#include <memory>

using namespace std;

class Application {
        ProducerConsumerMonitor _monitor;
        unique_ptr<Entity> _entity;
    
        void do_something();
    public:
        Application(int argc, char *argv[]);
        void run();
};

#endif /* APPLICATION_H */
