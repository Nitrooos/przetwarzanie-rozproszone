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
    return "[RULE PLACEHHOLDER]";
}
