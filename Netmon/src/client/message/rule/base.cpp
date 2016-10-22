#include "base.hpp"
#include "new.hpp"
#include "del.hpp"

#include "../../exceptions/types.hpp"

#include <sstream>
#include <cstring>
#include <arpa/inet.h>

Message::Rule::Base::Base(struct nlmsghdr *header): Message::Base(header) {
    this->set_attributes(RTM_RTA((struct rtmsg*)NLMSG_DATA(header)));
}

Message::Rule::Base *Message::Rule::Base::build(struct nlmsghdr *header) {
    switch (header->nlmsg_type) {
        case RTM_NEWRULE:
            return new Message::Rule::New(header);
        case RTM_DELRULE:
            return new Message::Rule::Del(header);
    }
}

string Message::Rule::Base::shout() {
    auto msg = (struct rtmsg*)NLMSG_DATA(this->_header);
    return "\n" + this->family(msg->rtm_family) +
                  this->flags(msg->rtm_flags) +
                  this->attributes();
}

string Message::Rule::Base::family(unsigned char family) const {
    ostringstream S;
    string family_name;

    switch (family) {
        case AF_INET:  family_name = "IPv4"; break;
        case AF_INET6: family_name = "IPv6"; break;
        default:       family_name = "Unknown";
    }
    this->print(S, "address family", family_name);
    return S.str();
}

string Message::Rule::Base::flags(unsigned int flags) const {
    ostringstream S;
    if (flags != 0) {
        this->print(S, "flags");
        this->print(S, flags, RTM_F_NOTIFY,     "RTM_F_NOTIFY",   "if the route changes, notify the user via rtnetlink");
        this->print(S, flags, RTM_F_CLONED,     "RTM_F_CLONED",   "route is cloned from another route"                 );
        this->print(S, flags, RTM_F_EQUALIZE,   "RTM_F_EQUALIZE", "multipath equalizer"                                );
    }
    return S.str();
}

string Message::Rule::Base::attributes() const {
    ostringstream S;
    if (!this->_attributes.empty()) {
        this->print(S, "attributes");
        for (auto attr : this->_attributes) {
            const char *inet;
            char str[32] = "";
            switch (attr->rta_type) {
                case RTA_DST:
                    inet = inet_ntop(AF_INET, RTA_DATA(attr), str, sizeof str);
                    if (inet == nullptr) {
                        throw Exception::Warning(strerror(errno));
                    }
                    this->print(S, "to", str, 2);
                    break;
                case RTA_SRC:
                    inet = inet_ntop(AF_INET, RTA_DATA(attr), str, sizeof str);
                    if (inet == nullptr) {
                        throw Exception::Warning(strerror(errno));
                    }
                    this->print(S, "from", str, 2);
                    break;
                case RTA_IIF:
                    if (strncpy(str, (char*) RTA_DATA(attr), sizeof str) == nullptr) {
                        throw Exception::Warning("Rule::shout() Cannot read dev index");
                    }
                    this->print(S, "from dev", str, 2);
                    break;
                case RTA_OIF:
                    if (strncpy(str, (char*) RTA_DATA(attr), sizeof str) == nullptr) {
                        throw Exception::Warning("Rule::shout() Cannot read dev index");
                    }
                    this->print(S, "to dev", str, 2);
                    break;
                case RTA_PRIORITY:
                    this->print(S, "priority", to_string(*((int*) RTA_DATA(attr))), 2);
                    break;
                case RTA_TABLE:
                    unsigned char table;
                    memcpy(&table, RTA_DATA(attr), 1);
                    this->print(S, "table", this->get_table_name(table), 2);
                    break;
            }
        }
    }
    return S.str();
}

string Message::Rule::Base::get_table_name(unsigned char table) const {
    switch (table) {
        case 255: return "local";
        case 254: return "main";
        case 253: return "default";
        case   0: return "unspec";
        default:  return to_string(table);
    }
}
