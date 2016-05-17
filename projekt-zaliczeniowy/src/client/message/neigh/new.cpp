#include "new.hpp"

Message::Neigh::New::New(struct nlmsghdr *header) : Message::Neigh::Base(header) { }

string Message::Neigh::New::shout() {
    return "NEIGH NEW " + Neigh::Base::shout();
}
