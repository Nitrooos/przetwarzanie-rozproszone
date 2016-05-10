#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>

#include "connection/tcp.hpp"
#include "connection/kernel.hpp"

using namespace std;

class Application {
    private:
        Application(int argc, char *argv[]);
        
        static unique_ptr<Application> _instance;
        unique_ptr<Connection::TCP> _server_connection;
        unique_ptr<Connection::Kernel> _kernel_connection;
    public:
        static Application *get_instance(int argc, char *argv[]);
        void run();
};

#endif
