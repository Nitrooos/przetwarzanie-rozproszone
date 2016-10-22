#include "operations.hpp"
#include "types.hpp"

void Lock::clone(FILE_LOCK &to, FILE_LOCK &from) {
    strcpy(to._filename, from._filename);
    to._type = from._type;
    to._start = from._start;
    to._length = from._length;
}

bool Lock::blocking(FILE_LOCK &l1, FILE_LOCK &l2) {
    if (l1._type == Lock::Shared && l2._type == Lock::Shared) {
        return false;
    }
    
    if (l2._start > l1._start) {
        return l2._start < l1._start + l1._length;
    } else {
        return l1._start < l2._start + l2._length;
    }
}

string Lock::visualise(FILE_LOCK &l, string const& text, int start) {
    if (l._start < start) {
        return "\033[3" + to_string(l._type) + "m" + text.substr(start, max(0, l._length - (start - l._start))) + "\033[m";
    } else {
        return "\033[3" + to_string(l._type) + "m" + text.substr(l._start, l._length) + "\033[m";
    }
}
