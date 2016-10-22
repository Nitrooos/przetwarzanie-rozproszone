#include "del.hpp"

Message::Route::Del::Del(struct nlmsghdr *header) : Message::Route::Base(header) { }

string Message::Route::Del::shout() {
    return "ROUTE DEL " + Route::Base::shout();
}
