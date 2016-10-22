#include "del.hpp"

Message::Link::Del::Del(struct nlmsghdr *header) : Message::Link::Base(header) { }

string Message::Link::Del::shout() {
    return "LINK DEL " + Link::Base::shout();
}
