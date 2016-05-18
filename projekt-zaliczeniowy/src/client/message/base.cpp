#include "base.hpp"
#include "route/base.hpp"
#include "link/base.hpp"
#include "addr/base.hpp"
#include "neigh/base.hpp"

#include "../exceptions/types.hpp"

#include <sstream>
#include <cstring>
#include <iomanip>

Message::Base* Message::Base::build(struct nlmsghdr *header) {
    switch (header->nlmsg_type) {
        case RTM_NEWROUTE: case RTM_DELROUTE:
            return Message::Route::Base::build(header);
        case RTM_NEWLINK: case RTM_DELLINK:
            return Message::Link::Base::build(header);
        case RTM_NEWADDR: case RTM_DELADDR:
            return Message::Addr::Base::build(header);
        case RTM_NEWNEIGH: case RTM_DELNEIGH:
            return Message::Neigh::Base::build(header);
        default:
            throw Exception::Warning("Unknown message type: " + header->nlmsg_type);
    }
}

Message::Base::Base(struct nlmsghdr *header) : _header(header) { }

void Message::Base::print(ostream & S, int state, int flag, string flag_as_string, string info) const {
    if (state & flag) {
        S << setw(25) << flag_as_string << "     " << info << "\n";
    }
}

void Message::Base::print(ostream & S, string flag_as_string, string value, int indent) const {
    S << string(indent*3, ' ') << flag_as_string << ":\n" << setw(25) << value << "\n";
}

void Message::Base::print(ostream & S, string header, int indent) const {
    S << string(indent*3, ' ') << header << ":\n";
}
