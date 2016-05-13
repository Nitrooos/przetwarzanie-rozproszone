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

Message::Base::Base(struct nlmsghdr *header) {
    this->_header = *header;
    this->_rtnetlink_message = *(struct rtmsg*)NLMSG_DATA(header);
    
    struct rtattr *atp = (struct rtattr*) RTM_RTA(&this->_rtnetlink_message);
    int atlen = RTM_PAYLOAD(header);
    for(;RTA_OK(atp, atlen); atp = RTA_NEXT(atp, atlen)) {
        this->_attributes.push_back(*atp);
    }    
}
