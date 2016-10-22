#include "access.hpp"
#include "../state/store.hpp"
#include "../../shared/lock/operations.hpp"

#include <unistd.h>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <list>
#include <algorithm>

bool File::exists(const char *name) {
    return (access(name, F_OK) == 0);
}

string File::content(const char *name) {
    ifstream t(name);
    return string((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
}

void File::visualise(string const& content, State::Locks const& map_locks) {
    list<FILE_LOCK> locks;
    transform(map_locks.begin(), map_locks.end(), back_inserter(locks),
        [] (State::Locks::value_type const& val) {
            return val.second;
        }
    );
    
    locks.sort([](FILE_LOCK &l1, FILE_LOCK &l2) {
        return l1._start < l2._start;
    });
    
    // czyszczenie ekranu
    cout << "\033[2J\033[1;1H";
    
    int start = 0;
    for (auto &l : locks) {
        cout << content.substr(start, max(0, l._start - start));
        cout << Lock::visualise(l, content, start);
        start = max(start, l._start + l._length);
    }
    
    cout << content.substr(start, content.length() - start) << endl;
}

int File::check_lock(const char *name, int start, int length) {
    if (File::exists(name)) {
        if (start < 0) {
            return Lock::Status::Invalid;
        }
        
        int file_length = File::content(name).length();
        if (start + length >= file_length) {
            return Lock::Status::Invalid;
        }
        
        return Lock::Status::OK;
    } else {
        return Lock::Status::NoSuchFile;
    }
}
