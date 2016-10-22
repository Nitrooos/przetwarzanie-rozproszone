#ifndef PARSE_H
#define PARSE_H

#include <string>

using namespace std;

struct Arguments {
    string _server, _filename;
    int _type, _start, _length;
    
    static Arguments parse(int argc, char *argv[]);
};

#endif
