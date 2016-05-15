#include "base.hpp"

#include <cstring>

#include "route/new.hpp"
#include "route/del.hpp"

Message::Base* Message::Base::build(struct nlmsghdr *header) {
    switch (header->nlmsg_type) {
        case RTM_NEWROUTE:
            return new Message::Route::New(header);
            break;
        case RTM_DELROUTE:
            return new Message::Route::Del(header);
            break;
    }
}

Message::Base::Base(struct nlmsghdr *header) : _header(header) { }
