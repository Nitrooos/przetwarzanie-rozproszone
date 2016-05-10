#include "parse.hpp"
#include "../exceptions/types.hpp"

#include <stdexcept>
#include <cstring>

ServerAddress Arguments::parse(int argc, char *argv[]) {
    ServerAddress addr;
    
    switch (argc) {
        case 1:
            throw Exception::Warning("You didn't specified server IP address nor port so all notifications will be only printed to local stdout");
            break;
        case 2:
            Arguments::parse_help(string(argv[1]));
            addr._ip_address = Arguments::parse_ip_address(string(argv[1]));
            addr._port = 1234;
            
            throw Exception::Warning("You didn't specified server port so default [1234] will be used");
            break;
        default:
            addr._ip_address = Arguments::parse_ip_address(string(argv[1]));
            addr._port = Arguments::parse_port_number(string(argv[2]));
            break;
    }
    
    return addr;
}


void Arguments::parse_help(string const& arg) {
    if (arg == "-h" || arg == "--help") {
        throw Exception::Help();
    }
}

struct in_addr Arguments::parse_ip_address(string const& arg) {
    unsigned char buf[sizeof(struct in_addr)];
    if (inet_pton(AF_INET, arg.c_str(), buf) != 1) {
        throw Exception::Warning("'" + arg + "' is not a valid IP address so it will be ignored");
    }
    
    return *(struct in_addr*)buf;
}

int Arguments::parse_port_number(string const& arg) {
    try {
        int port = stoi(arg);
        if (port < 1024 || port > 32767) {
            throw Exception::Error(arg + " is not in allowed range 1024 .. 32767");
        }
    } catch (invalid_argument const& e) {
        throw Exception::Error(arg + " is not a valid port number, try something between 1024 .. 32767");
    } catch (out_of_range const& e) {
        throw Exception::Error(arg + " is not in allowed range 1024 .. 32767");
    }
}
