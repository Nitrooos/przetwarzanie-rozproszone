#ifndef PARSE_H
#define PARSE_H

#include <string>

#include "../connection/address.hpp"

using namespace std;

class Arguments {
        static void set_address(TCPAddress &address, int argc, char *argv[]);
    
        static void parse_help(string const& arg);
        static struct in_addr parse_ip_address(string const& arg);
        static int parse_port_number(string const& arg);
    
    public:
        static TCPAddress parse(int argc, char *argv[]);
};

#endif
