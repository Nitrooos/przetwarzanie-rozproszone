#include "base.hpp"
#include "new.hpp"
#include "del.hpp"

Message::Link::Base::Base(struct nlmsghdr *header): Message::Base(header) {
    this->set_attributes(IFLA_RTA((struct ifinfomsg*)NLMSG_DATA(header)));
}

string Message::Link::Base::shout() {
    return "[LINK PLACEHOLDER]";
}

Message::Link::Base *Message::Link::Base::build(struct nlmsghdr *header) {
    switch (header->nlmsg_type) {
        case RTM_NEWLINK:
            return new Message::Link::New(header);
        case RTM_DELLINK:
            return new Message::Link::Del(header);
    }
}
