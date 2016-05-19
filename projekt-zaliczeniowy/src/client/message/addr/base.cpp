#include "base.hpp"
#include "new.hpp"
#include "del.hpp"

#include <sstream>
#include <arpa/inet.h>

Message::Addr::Base::Base(struct nlmsghdr *header): Message::Base(header) {
    this->set_attributes((struct ifaddrmsg*)NLMSG_DATA(header));
}

string Message::Addr::Base::shout() {
    auto msg = (struct ifaddrmsg*)NLMSG_DATA(this->_header);
    return "\n" + this->family(msg->ifa_family) +
                  this->flags(msg->ifa_flags) +
                  this->device(msg->ifa_index) +
                  this->mask(msg->ifa_prefixlen);
}

Message::Addr::Base *Message::Addr::Base::build(struct nlmsghdr *header) {
    switch (header->nlmsg_type) {
        case RTM_NEWADDR:
            return new Message::Addr::New(header);
        case RTM_DELADDR:
            return new Message::Addr::Del(header);
    }
}

string Message::Addr::Base::flags(unsigned char flags) const {
    ostringstream S;
    if (flags != 0) {
        this->print(S, "flags");
        this->print(S, flags, IFA_F_SECONDARY, "IFA_F_SECONDARY", "secondary address (old alias interface)");
        this->print(S, flags, IFA_F_PERMANENT, "IFA_F_PERMANENT", "permanent address set by the user"      );
    }
    return S.str();
}

string Message::Addr::Base::family(unsigned char family) const {
    ostringstream S;
    string family_name;
    
    switch (family) {
        case AF_INET:  family_name = "AF_INET";  break;
        case AF_INET6: family_name = "AF_INET6"; break;
        default:       family_name = "Unknown";
    }    
    this->print(S, "address family", family_name);
    return S.str();
}

string Message::Addr::Base::mask(unsigned char mask) const {
    ostringstream S;
    this->print(S, "mask", "/" + to_string(mask));
    return S.str();
}
