#ifndef SYNCHRONIZER_H
#define SYNCHRONIZER_H

#include <mutex>
#include <condition_variable>

using namespace std;

namespace Monitor {

    struct Synchronizer {
        mutex _mutex;                       // zamek
        condition_variable _cond_variable;  // zmienna warunkowa
        bool _ready{false};                 // czy warunek kontynuacji przetwarzania jest spełniony? na początku oczywiście nie
    };
    
}

#endif /* SYNCHRONIZER_H */
