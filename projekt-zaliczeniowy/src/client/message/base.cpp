#include "base.hpp"

#include <cstring>

#include "route/new.hpp"
#include "route/del.hpp"

#include <iostream>
Message::Base* Message::Base::build(struct nlmsghdr *header) {
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
        default:
            std::cout << "Unknown message type: " << header->nlmsg_type << "\n";
            return nullptr;
    }
}

Message::Base::Base(struct nlmsghdr *header) : _header(header) { }
