#include "application.hpp"
#include <monitor/exceptions.hpp>
#include <iostream>
#include <unistd.h>

Application::Application(int argc, char *argv[]) {
    this->_entity.reset(Entity::create(this->_monitor));
}

void Application::do_something() {
    // symuluje przetwarzanie aplikacyjne o nieokreślonym czasie trwania
    int microseconds = rand()%3000 + 2000;
    usleep(microseconds);
}

void Application::run() {
    try {
        while (true) {
            this->do_something();
            this->_entity->do_your_job();
        }
    } catch (MPIALreadyInitialized const& e)          { cerr << e.what(); } 
      catch (CannotCreateThread const& e)             { cerr << e.what(); } 
      catch (NotSufficientConcurrencyLevel const& e)  { cerr << e.what(); }
      catch (...) {
        cerr << "Unknown exception was thrown!\n";
    }
}
