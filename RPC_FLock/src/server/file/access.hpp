#ifndef ACCESS_H
#define ACCESS_H

#include <string>
#include <map>

#include "../../shared/RPC_FLock.hpp"

using namespace std;

namespace File {    
    bool exists(const char *name);
    string content(const char *name);
    int check_lock(const char *name, int start, int length);
    void visualise(string const& content, map<int, FILE_LOCK> const& locks);
}

#endif
