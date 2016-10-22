#include "entities.hpp"

Entity::Entity(ProducerConsumerMonitor &monitor) : _monitor(monitor) { }

Entity* Entity::create(ProducerConsumerMonitor &monitor) {
    // tworzymy (mniej więcej) tyle samo producentów co konsumentów - można zmieniać do eksperymentów
    if (monitor.get_tid() % 20 < 10) {
        return new Producer(monitor);
    } else {
        return new Consumer(monitor);
    }
}

Producer::Producer(ProducerConsumerMonitor &monitor) : Entity(monitor) { }

void Producer::do_your_job() {
    this->_monitor.produce();
}

Consumer::Consumer(ProducerConsumerMonitor &monitor) : Entity(monitor) { }

void Consumer::do_your_job() {
    this->_monitor.consume();
}
