#include "base.hpp"
#include "new.hpp"
#include "del.hpp"

#include <cstring>
#include <net/if.h>
#include <arpa/inet.h>
#include <sstream>

Message::Link::Base::Base(struct nlmsghdr *header): Message::Base(header) {
    this->set_attributes(IFLA_RTA((struct ifinfomsg*)NLMSG_DATA(header)));
}

string Message::Link::Base::shout() {
    auto msg = (struct ifinfomsg*)NLMSG_DATA(this->_header);
    return "\n" + this->flags(msg->ifi_flags) +
                  this->attributes();
}

Message::Link::Base *Message::Link::Base::build(struct nlmsghdr *header) {
    switch (header->nlmsg_type) {
        case RTM_NEWLINK:
            return new Message::Link::New(header);
        case RTM_DELLINK:
            return new Message::Link::Del(header);
    }
}

string Message::Link::Base::flags(unsigned int flags) const {
    ostringstream S;
    if (flags != 0) {
        this->print(S, "flags");
        this->print(S, flags, IFF_UP,        "IFF_UP",        "interface is running"             );
        this->print(S, flags, IFF_BROADCAST, "IFF_BROADCAST", "valid broadcast address set"      );
        this->print(S, flags, IFF_LOOPBACK,  "IFF_LOOPBACK",  "interface is a loopback interface");
        this->print(S, flags, IFF_PROMISC,   "IFF_PROMISC",   "interface is in promiscuous mode.");
        this->print(S, flags, IFF_RUNNING,   "IFF_RUNNING",   "resources allocated"              );
        this->print(S, flags, IFF_MULTICAST, "IFF_MULTICAST", "supports multicast"               );
    }
    return S.str();
}

string Message::Link::Base::attributes() const {
    ostringstream S;
    if (!this->_attributes.empty()) {
        this->print(S, "attributes");
        for (auto attr : this->_attributes) {
            const char *inet;
            switch (attr->rta_type) {
                case IFLA_ADDRESS:
                    unsigned char _hwaddr[6];
                    memcpy(_hwaddr, RTA_DATA(attr), sizeof _hwaddr);
                    this->print(S, "hwaddr", this->make_hwaddr_string(_hwaddr), 2);
                    break;
                case IFLA_IFNAME:
                    char _interface[32];
                    memcpy(_interface, RTA_DATA(attr), sizeof _interface);
                    this->print(S, "dev", _interface, 2);
                    break;
                case IFLA_MTU:
                    int _mtu;
                    memcpy(&_mtu, RTA_DATA(attr), sizeof _mtu);
                    this->print(S, "MTU", to_string(_mtu), 2);
                    break;
            }
        }
    }
    return S.str();
}
