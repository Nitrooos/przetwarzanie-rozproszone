#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>

using namespace std;

class Application {
    private:
        Application(int argc, char *argv[]);
        
        static unique_ptr<Application> _instance;
    public:
        static Application *get_instance(int argc, char *argv[]);
        
        void run();
};

#endif
