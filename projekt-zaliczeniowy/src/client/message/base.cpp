#include "base.hpp"
#include "route/base.hpp"
#include "link/base.hpp"
#include "addr/base.hpp"
#include "neigh/base.hpp"

#include <cstring>
#include <iomanip>

#include <iostream>
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
            std::cout << "Unknown message type: " << header->nlmsg_type << "\n";
            return nullptr;
    }
}

Message::Base::Base(struct nlmsghdr *header) : _header(header) { }

void Message::Base::print(ostream & S, int state, int flag, string flag_as_string, string info) const {
    if (state & flag) {
        S << setw(20) << flag_as_string << "     " << info << "\n";
    }
}

void Message::Base::print(ostream & S, string flag_as_string, string value) const {
    S << "   " << flag_as_string << ":\n" << setw(20) << value << "\n";
}

void Message::Base::print(ostream & S, string header) const {
    S << "   " << header << ":\n";
}
