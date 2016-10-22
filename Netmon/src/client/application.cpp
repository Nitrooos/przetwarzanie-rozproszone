#include "application.hpp"
#include "exceptions/types.hpp"
#include "arguments/parse.hpp"

#include <signal.h>
#include <unistd.h>
#include <iostream>

unique_ptr<Application> Application::_instance;

void handle(int signum) {
    cout << "\n\n[INFO] SIGINT detected, releasing resources...\n";
    exit(0);
}

Application::Application(int argc, char *argv[]) {
    signal(SIGINT, handle);
    
    this->_hostname = this->get_host_name();

    try {
        TCPAddress server_address = Arguments::parse(argc, argv);
        if (server_address._ip_address.s_addr != 0 && server_address._port != 0) {
            this->_server_connection.reset(new Connection::TCP(server_address));
            cout << "[INFO] Connected with server successfully\n";
        }
    } catch (Exception::NoHostName const& s) {
        this->_hostname = "noname";
        cerr << s.what();
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
        // odbierz kolejne wiadomości z jądra
        auto messages = this->_kernel_connection->receive();
        
        // wypisz je na ekran, ewentualnie wyślij do serwera
        for (auto &m : messages) {
            try {
                cout << "Message from " << this->_hostname << ": " << m->shout();
            } catch (Exception::Printable const& e) {
                cerr << e.what();
            }

            if (this->_server_connection.get() != nullptr) {
                this->_server_connection->send(this->_hostname + " says " + m->shout().c_str());
            }
        }
        
        // zwolnienie pamięci wiadomości
        for (auto &m : messages) {
            delete m;
        }
    }
}

string Application::get_host_name() {
    char hostname[30];
    if (gethostname(hostname, 30) != 0) {
        throw Exception::NoHostName();
    }
    
    return hostname;
}
