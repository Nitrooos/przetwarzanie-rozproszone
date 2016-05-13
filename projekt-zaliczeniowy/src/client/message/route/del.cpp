#include "del.hpp"

Message::Route::Del::Del(struct nlmsghdr *header) : Base(header) { }

string Message::Route::Del::shout() {
    return "[DEL]";
}
