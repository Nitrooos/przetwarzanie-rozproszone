#include "new.hpp"

Message::Link::New::New(struct nlmsghdr *header) : Message::Link::Base(header) { }

string Message::Link::New::shout() {
    return "LINK NEW " + Link::Base::shout();
}
