#include "del.hpp"

Message::Neigh::Del::Del(struct nlmsghdr *header) : Message::Neigh::Base(header) { }

string Message::Neigh::Del::shout() {
    return "NEIGHBOUR DELETE" + Neigh::Base::shout();
}
