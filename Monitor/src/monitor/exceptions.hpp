#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

using namespace std;

struct MPIALreadyInitialized : public exception {
    const char *what() const throw() {
        return string("[error] Tried to call MPI_Init when MPI was already initialized!\n").c_str();
    }
};

struct CannotCreateThread : public exception {
    const char *what() const throw() {
        return string("[error] Cannot create new monitor thread with pthread_create!\n").c_str();
    }
};

// BARDZO WAŻNE! Jeśli użyty do kompilacji MPI nie wspiera wielowątkowości, to wystąpi ten wyjątek!
struct NotSufficientConcurrencyLevel : public exception {
    const char *what() const throw() {
        return string("[error] Cannot obtain concurrency level MPI_THREAD_MULTIPLE which is required!\n").c_str();
    }
};

#endif /* EXCEPTIONS_H */
