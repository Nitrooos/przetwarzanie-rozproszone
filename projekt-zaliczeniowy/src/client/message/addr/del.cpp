#include "del.hpp"

Message::Addr::Del::Del(struct nlmsghdr *header) : Message::Addr::Base(header) { }

string Message::Addr::Del::shout() {
    return "ADDR DEL " + Addr::Base::shout();
}
