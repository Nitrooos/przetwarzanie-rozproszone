#include "base.hpp"
#include "new.hpp"
#include "del.hpp"

#include <sstream>

Message::Neigh::Base::Base(struct nlmsghdr *header): Message::Base(header) {
    this->set_attributes((struct ndmsg*)NLMSG_DATA(header));
}

string Message::Neigh::Base::shout() {
    auto msg = (struct ndmsg*)NLMSG_DATA(this->_header);
    return "\n" + this->state(msg->ndm_state) + 
                  this->flags(msg->ndm_flags) + 
                  this->device(msg->ndm_ifindex);
}

Message::Neigh::Base *Message::Neigh::Base::build(struct nlmsghdr *header) {
    switch (header->nlmsg_type) {
        case RTM_NEWNEIGH:
            return new Message::Neigh::New(header);
        case RTM_DELNEIGH:
            return new Message::Neigh::Del(header);
    }
}

string Message::Neigh::Base::state(__u16 state) const {
    ostringstream S;

    if (state != 0) {
        this->print(S, "state");        
        this->print(S, state, NUD_INCOMPLETE, "NUD_INCOMPLETE", "currently resolving cache entry"       );
        this->print(S, state, NUD_REACHABLE,  "NUD_REACHABLE",  "confirmed working cache entry"         );
        this->print(S, state, NUD_STALE,      "NUD_STALE",      "expired cache entry"                   );
        this->print(S, state, NUD_DELAY,      "NUD_DELAY",      "entry waiting for a timer"             );
        this->print(S, state, NUD_PROBE,      "NUD_PROBE",      "cache entry that is currently reprobed");
        this->print(S, state, NUD_FAILED,     "NUD_FAILED",     "invalid cache entry"                   );
        this->print(S, state, NUD_NOARP,      "NUD_NOARP",      "device with no destination cache"      );
        this->print(S, state, NUD_PERMANENT,  "NUD_PERMANENT",  "static entry"                          );
    }
    return S.str();
}

string Message::Neigh::Base::flags(__u8 flags) const {
    ostringstream S;
    
    if (flags != 0) {
        this->print(S, "flags");
        this->print(S, flags, NTF_PROXY,  "NTF_PROXY",  "proxy arp entry");
        this->print(S, flags, NTF_ROUTER, "NTF_ROUTER", "IPv6 router"    );
    }
    return S.str();
}

string Message::Neigh::Base::device(int device) const {
    ostringstream S;
    this->print(S, "device", to_string(device));
    return S.str();
}
