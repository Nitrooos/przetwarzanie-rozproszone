#include "base.hpp"
#include "new.hpp"
#include "del.hpp"

#include "../../exceptions/types.hpp"

#include <sstream>
#include <cstring>
#include <arpa/inet.h>

Message::Route::Base::Base(struct nlmsghdr *header): Message::Base(header) {
    this->set_attributes(RTM_RTA((struct rtmsg*)NLMSG_DATA(header)));
}

Message::Route::Base *Message::Route::Base::build(struct nlmsghdr *header) {
    switch (header->nlmsg_type) {
        case RTM_NEWROUTE:
            if (Message::Route::Base::validate_header(header)) {
                return new Message::Route::New(header);
            }
            return nullptr;
        case RTM_DELROUTE:
            if (Message::Route::Base::validate_header(header)) {
                return new Message::Route::Del(header);
            }
            return nullptr;
    }
}

bool Message::Route::Base::validate_header(struct nlmsghdr *header) {
    struct rtmsg *rtp = (struct rtmsg*) NLMSG_DATA(header);
    if (rtp->rtm_table != RT_TABLE_MAIN) {
        return false;
    }
    return true;
}

string Message::Route::Base::shout() {
    char msk[32];
    int sprintf_res;
    
    sprintf_res = sprintf(msk, "%d", ((struct rtmsg*)NLMSG_DATA(this->_header))->rtm_dst_len);
    if (sprintf_res < 0) {
        throw Exception::Warning("Route::New::shout() Cannot read msk");
    }
    
    AttributeValues attrs_vals = this->read_attributes();
    
    auto msg = (struct rtmsg*)NLMSG_DATA(this->_header);
    return "\n" + this->type(msg->rtm_type) +
                  this->protocol(msg->rtm_protocol) +
                  this->table(msg->rtm_table) +
                  this->scope(msg->rtm_scope) +
                  this->flags(msg->rtm_flags) +
                  this->attributes(attrs_vals);
}

Message::Route::Base::AttributeValues Message::Route::Base::read_attributes() const {
    AttributeValues attr_vals;
    for (auto &attr : this->_attributes) {
        const char *inet;
        switch(attr->rta_type) {
            case RTA_DST:
                inet = inet_ntop(AF_INET, RTA_DATA(attr), attr_vals._dst, sizeof attr_vals._dst);
                if (inet == nullptr) {
                    throw Exception::Warning(strerror(errno));
                }
                break;
            case RTA_SRC:
                inet = inet_ntop(AF_INET, RTA_DATA(attr), attr_vals._src, sizeof attr_vals._src);
                if (inet == nullptr) {
                    throw Exception::Warning(strerror(errno));
                }
                break;
            case RTA_GATEWAY:
                inet = inet_ntop(AF_INET, RTA_DATA(attr), attr_vals._gateway, sizeof attr_vals._gateway);
                if (inet == nullptr) {
                    throw Exception::Warning(strerror(errno));
                }
                break;
            case RTA_OIF:
                int sprintf_res = sprintf(attr_vals._dev, "%d", *((int*) RTA_DATA(attr)));
                if (sprintf_res < 0) {
                    throw Exception::Warning("Route::New::shout() Cannot read dev index");
                }
                break;
        }
    }    
    return attr_vals;
}

string Message::Route::Base::type(unsigned char type) const {
    ostringstream S;
    if (type != 0) {
        this->print(S, "type");        
        this->print(S, type, RTN_UNICAST,       "RTN_UNICAST",      "gateway or direct route"                    );
        this->print(S, type, RTN_LOCAL,         "RTN_LOCAL",        "local interface route"                      );
        this->print(S, type, RTN_BROADCAST,     "RTN_BROADCAST",    "local broadcast route (sent as a broadcast)");
        this->print(S, type, RTN_ANYCAST,       "RTN_ANYCAST",      "local broadcast route (sent as a unicast)"  );
        this->print(S, type, RTN_MULTICAST,     "RTN_MULTICAST",    "multicast route"                            );
        this->print(S, type, RTN_BLACKHOLE,     "RTN_BLACKHOLE",    "packet dropping route"                      );
        this->print(S, type, RTN_UNREACHABLE,   "RTN_UNREACHABLE",  "unreachable destination"                    );
        this->print(S, type, RTN_PROHIBIT,      "RTN_PROHIBIT",     "packet rejection route"                     );
        this->print(S, type, RTN_THROW,         "RTN_THROW",        "continue routing lookup in another table"   );
        this->print(S, type, RTN_NAT,           "RTN_NAT",          "network address translation rule"           );
        this->print(S, type, RTN_XRESOLVE,      "RTN_XRESOLVE",     "refer to an external resolver"              );
    }
    return S.str();
}

string Message::Route::Base::protocol(unsigned char protocol) const {
    ostringstream S;
    if (protocol != 0) {
        this->print(S, "protocol");
        this->print(S, protocol, RTPROT_REDIRECT,   "RTPROT_REDIRECT", "by an ICMP redirect" );
        this->print(S, protocol, RTPROT_KERNEL,     "RTPROT_KERNEL",   "by the kernel"       );
        this->print(S, protocol, RTPROT_BOOT,       "RTPROT_BOOT",     "during boot"         );
        this->print(S, protocol, RTPROT_STATIC,     "RTPROT_STATIC",   "by the administrator");
    }
    return S.str();
}

string Message::Route::Base::table(unsigned char table) const {
    ostringstream S;
    if (table != 0) {
        this->print(S, "table");
        this->print(S, table, RT_TABLE_DEFAULT, "RT_TABLE_DEFAULT", "the default table");
        this->print(S, table, RT_TABLE_MAIN,    "RT_TABLE_MAIN",    "the main table"   );
        this->print(S, table, RT_TABLE_LOCAL,   "RT_TABLE_LOCAL",   "the local table"  );
    }
    return S.str();
}

string Message::Route::Base::scope(unsigned char scope) const {
    ostringstream S;
    if (scope != 0) {
        this->print(S, "scope");
        this->print(S, scope, RT_SCOPE_UNIVERSE, "RT_SCOPE_UNIVERSE", "global route"                                 );
        this->print(S, scope, RT_SCOPE_SITE,     "RT_SCOPE_SITE",     "interior route in the local autonomous system");
        this->print(S, scope, RT_SCOPE_LINK,     "RT_SCOPE_LINK",     "route on this link"                           );
        this->print(S, scope, RT_SCOPE_HOST,     "RT_SCOPE_HOST",     "route on the local host"                      );
        this->print(S, scope, RT_SCOPE_NOWHERE,  "RT_SCOPE_NOWHERE",  "destination doesn't exist"                    );
    }
    return S.str();
}

string Message::Route::Base::flags(unsigned int flags) const {
    ostringstream S;
    if (flags != 0) {
        this->print(S, "flags");
        this->print(S, flags, RTM_F_NOTIFY,     "RTM_F_NOTIFY",   "if the route changes, notify the user via rtnetlink");
        this->print(S, flags, RTM_F_CLONED,     "RTM_F_CLONED",   "route is cloned from another route"                 );
        this->print(S, flags, RTM_F_EQUALIZE,   "RTM_F_EQUALIZE", "multipath equalizer"                                );
    }
    return S.str();
}

string Message::Route::Base::attributes(AttributeValues &attrs) const {
    ostringstream S;
    this->print(S, "attributes");
    if (strlen(attrs._src) > 0)       this->print(S, "source", attrs._src, 2);
    if (strlen(attrs._dst) > 0)       this->print(S, "destination", attrs._dst, 2);
    if (strlen(attrs._gateway) > 0)   this->print(S, "gateway", attrs._gateway, 2);
    if (strlen(attrs._dev) > 0)       this->print(S, "dev", attrs._dev, 2);
    return S.str();
}
