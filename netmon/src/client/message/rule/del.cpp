#include "del.hpp"

Message::Rule::Del::Del(struct nlmsghdr *header) : Message::Rule::Base(header) { }

string Message::Rule::Del::shout() {
    return "RULE DEL " + Rule::Base::shout();
}
