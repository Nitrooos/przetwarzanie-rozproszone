#include "new.hpp"

Message::Addr::New::New(struct nlmsghdr *header) : Message::Addr::Base(header) { }

string Message::Addr::New::shout() {
    return "ADDR NEW " + Addr::Base::shout();
}
