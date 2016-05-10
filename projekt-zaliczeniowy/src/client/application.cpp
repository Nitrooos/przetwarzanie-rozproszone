#include "application.hpp"
#include "arguments/parse.hpp"
#include "exceptions/types.hpp"

#include <iostream>

unique_ptr<Application> Application::_instance;

Application::Application(int argc, char *argv[]) {
    try {
        Arguments::parse(argc, argv);
    } catch (Exception::Printable const& s) {
        cerr << s.what();
    } catch (Exception::Help const& h) {
        cerr << h.what();
    }
}

Application *Application::get_instance(int argc, char *argv[]) {
    if (Application::_instance.get() == nullptr) {
        Application::_instance.reset(new Application(argc, argv));
    }
    return Application::_instance.get();
}

void Application::run() {
    
}
