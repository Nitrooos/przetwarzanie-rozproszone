#include "base.hpp"
#include "new.hpp"
#include "del.hpp"

Message::Addr::Base::Base(struct nlmsghdr *header): Message::Base(header) {
    this->set_attributes((struct ifaddrmsg*)NLMSG_DATA(header));
}

string Message::Addr::Base::shout() {
    auto msg = (struct ifaddrmsg*)NLMSG_DATA(this->_header);
    return "msk " + to_string(msg->ifa_prefixlen) + " dev " + to_string(msg->ifa_index);
}

Message::Addr::Base *Message::Addr::Base::build(struct nlmsghdr *header) {
    switch (header->nlmsg_type) {
        case RTM_NEWADDR:
            return new Message::Addr::New(header);
        case RTM_DELADDR:
            return new Message::Addr::Del(header);
    }
}
