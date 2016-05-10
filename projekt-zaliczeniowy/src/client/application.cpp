#include "application.hpp"
#include "exceptions/types.hpp"
#include "arguments/parse.hpp"

#include <iostream>

unique_ptr<Application> Application::_instance;

Application::Application(int argc, char *argv[]) {
    try {
        TCPAddress server_address = Arguments::parse(argc, argv);
        if (server_address._ip_address.s_addr != 0 && server_address._port != 0) {
            this->_server_connection.reset(new Connection::TCP(server_address));
            cout << "[INFO] Connected with server successfully\n";
        }
    } catch (Exception::Printable const& s) {
        cerr << s.what();
    } catch (Exception::Help const& h) {
        cerr << h.what();
    }
    
    this->_kernel_connection.reset(new Connection::Kernel());
}

Application *Application::get_instance(int argc, char *argv[]) {
    if (Application::_instance.get() == nullptr) {
        Application::_instance.reset(new Application(argc, argv));
    }
    return Application::_instance.get();
}

void Application::run() {
    while (true) {
        // odbierz kolejną wiadomość z jądra
        this->_kernel_connection->receive();
        cout << "Odebrano wiadomość\n";
        
        // jeśli jesteś połączony z serwerem to wyślij mu tą wiadomość
        if (this->_server_connection.get() != nullptr) {
            this->_server_connection->send("Odebrano wiadomość");
        }
    }
}
