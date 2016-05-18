#include "del.hpp"

Message::Neigh::Del::Del(struct nlmsghdr *header) : Message::Neigh::Base(header) { }

string Message::Neigh::Del::shout() {
    return "DELETE NEIGHBOUR " + Neigh::Base::shout();
}
