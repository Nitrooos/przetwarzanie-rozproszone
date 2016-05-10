#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <exception>

namespace Exception {

    using namespace std;

    struct Printable : public exception {
        Printable(string prefix, string message) : _prefix(prefix), _msg(message) { }
        const char *what() const throw() {
            return string(this->_prefix + " " + this->_msg + "\n").c_str();
        }
        
        protected:
            string _prefix, _msg;
    };

    struct Warning : public Printable {
        Warning(string msg) : Printable("[WARNING]", msg) { }
    };
    
    struct Error : public Printable {
        Error(string msg) : Printable("[ERROR]", msg) { }
    };
    
    struct Help : public exception {
        const char *what() const throw() {
            return string("\nUsage:"
                          "\n    netmon [SERVER_IPv4 [SERVER_PORT]]"
                          "\n"
                          "\nServer IP and port are optional, but if not specified, logs will not be sent to central server, only print to stdout"
                          "\nIf port is not specified, default 1234 will be used"
                          "\n"
            ).c_str();
        }
    };

}

#endif
