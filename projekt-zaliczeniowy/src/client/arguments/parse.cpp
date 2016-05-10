#include "parse.hpp"
#include "../exceptions/types.hpp"

#include <iostream>
#include <stdexcept>
#include <cstring>

TCPAddress Arguments::parse(int argc, char *argv[]) {
    TCPAddress addr{0, 0};
    try {
        Arguments::set_address(addr, argc, argv);
    } catch (Exception::Warning const& w) {
        cerr << w.what();
    }
    return addr;
}

void Arguments::set_address(TCPAddress &address, int argc, char *argv[]) {
    switch (argc) {
        case 1:
            throw Exception::Warning("You didn't specified server IP address nor port so all notifications will be only printed to local stdout");
            break;
        case 2:
            Arguments::parse_help(string(argv[1]));
            address._ip_address = Arguments::parse_ip_address(string(argv[1]));
            address._port = htons(1234);
            
            throw Exception::Warning("You didn't specified server port so default [1234] will be used");
            break;
        default:
            address._ip_address = Arguments::parse_ip_address(string(argv[1]));
            address._port = Arguments::parse_port_number(string(argv[2]));
            break;
    }
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
            throw Exception::Error(arg + " is not allowed port number, try something between 1024 .. 32767");
        }
        return htons(port);
    } catch (invalid_argument const& e) {
        throw Exception::Error(arg + " is not a valid port number, try something between 1024 .. 32767");
    } catch (out_of_range const& e) {
        throw Exception::Error(arg + " is not in allowed range 1024 .. 32767");
    }
    return 0;
}
