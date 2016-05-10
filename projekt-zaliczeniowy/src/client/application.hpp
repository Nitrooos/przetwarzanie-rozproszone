#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>

#include "connection/tcp.hpp"

using namespace std;

class Application {
    private:
        Application(int argc, char *argv[]);
        
        static unique_ptr<Application> _instance;
        unique_ptr<Connection> _server_connection;
    public:
        static Application *get_instance(int argc, char *argv[]);
        void run();
};

#endif
