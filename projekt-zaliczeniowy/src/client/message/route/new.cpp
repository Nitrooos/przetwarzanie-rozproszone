#include "new.hpp"

Message::Route::New::New(struct nlmsghdr *header) : Base(header) { }

string Message::Route::New::shout() {
    return "[ADD]";
}
