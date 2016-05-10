#ifndef PARSE_H
#define PARSE_H

#include <arpa/inet.h>
#include <string>

#include "server-address.hpp"

using namespace std;

class Arguments {
        static void parse_help(string const& arg);
        static struct in_addr parse_ip_address(string const& arg);
        static int parse_port_number(string const& arg);
    
    public:
        static ServerAddress parse(int argc, char *argv[]);
};

#endif
