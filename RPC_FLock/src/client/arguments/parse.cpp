#include "parse.hpp"
#include "../../shared/lock/types.hpp"
#include "../../shared/file/default.hpp"

Arguments Arguments::parse(int argc, char *argv[]) {
    Arguments args;
    
    if (argc < 2) {
        throw "Usage: " + string(argv[0]) + " server_host [file_to_lock] [type] [start_byte] [length]";
    }
    
    args._server = argv[1];
    
    if (argc > 2) {
        args._filename = argv[2];
    } else {
        args._filename = File::DefaultFile;
    }
    
    if (argc > 3) {
        args._type = stoi(argv[3]);
    } else {
        args._type = Lock::Shared;
    }
    
    if (argc > 4) {
        args._start = stoi(argv[4]);
    } else {
        args._start = 0;
    }
    
    if (argc > 5) {
        args._length = stoi(argv[5]);
    } else {
        args._length = 10;
    }

    return args;
}
