#include "new.hpp"

Message::Route::New::New(struct nlmsghdr *header) : Message::Route::Base(header) { }

string Message::Route::New::shout() {
    return "[ROUTE ADD] " + Route::Base::shout();
}
