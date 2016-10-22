#include "base.hpp"
#include "new.hpp"
#include "del.hpp"

#include <cstring>
#include <sstream>
#include <arpa/inet.h>

Message::Addr::Base::Base(struct nlmsghdr *header): Message::Base(header) {
    this->set_attributes(IFA_RTA((struct ifaddrmsg*)NLMSG_DATA(header)));
}

string Message::Addr::Base::shout() {
    auto msg = (struct ifaddrmsg*)NLMSG_DATA(this->_header);
    return "\n" + this->family(msg->ifa_family) +
                  this->flags(msg->ifa_flags) +
                  this->attributes(msg->ifa_prefixlen);
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

string Message::Addr::Base::attributes(unsigned char mask) const {
    ostringstream S;
    if (!this->_attributes.empty()) {
        this->print(S, "attributes");
        for (auto attr : this->_attributes) {
            const char *inet;
            switch (attr->rta_type) {
                case IFA_ADDRESS:
                    char _address[32];
                    inet = inet_ntop(AF_INET, RTA_DATA(attr), _address, sizeof _address);
                    if (inet != nullptr) {
                        this->print(S, "address", string(_address) + "/" + to_string(mask), 2);
                    }
                    break;
                case IFA_LABEL:
                    char _interface[32];
                    memcpy(_interface, RTA_DATA(attr), sizeof _interface);
                    this->print(S, "dev", _interface, 2);
                    break;
            }
        }
    }
    return S.str();
}
