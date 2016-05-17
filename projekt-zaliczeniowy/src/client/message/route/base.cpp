#include "base.hpp"
#include "new.hpp"
#include "del.hpp"

#include "../../exceptions/types.hpp"

#include <cstring>
#include <arpa/inet.h>

Message::Route::Base::Base(struct nlmsghdr *header): Message::Base(header) {
    this->set_attributes((struct rtmsg*)NLMSG_DATA(header));
}

string Message::Route::Base::shout() {
    char dev[32] = "", dst[32] = "", msk[32] = "", gwy[32] = "";
    int sprintf_res;
    
    for (auto &attr : this->_attributes) {
        const char *inet;
        switch(attr->rta_type) {
            case RTA_DST:
                inet = inet_ntop(AF_INET, RTA_DATA(attr), dst, sizeof(dst));
                if (inet == nullptr) {
                    throw Exception::Warning(strerror(errno));
                }
                break;
            case RTA_GATEWAY:
                inet = inet_ntop(AF_INET, RTA_DATA(attr), gwy, sizeof(gwy));
                if (inet == nullptr) {
                    throw Exception::Warning(strerror(errno));
                }
                break;
            case RTA_OIF:
                sprintf_res = sprintf(dev, "%d", *((int*) RTA_DATA(attr)));
                if (sprintf_res < 0) {
                    throw Exception::Warning("Route::New::shout() Cannot read dev index");
                }
                break;
        }
    }
    
    sprintf_res = sprintf(msk, "%d", ((struct rtmsg*)NLMSG_DATA(this->_header))->rtm_dst_len);
    if (sprintf_res < 0) {
        throw Exception::Warning("Route::New::shout() Cannot read msk");
    }
    
    if (strlen(dst) == 0) {
        return "default via " + string(gwy) + " dev " + string(dev);
    } else if (strlen(gwy) == 0) {
        return string(dst) + "/" + string(msk) + " dev " + string(dev);
    } else {
        return "dst " + string(dst) + "/" + string(msk) + " gateway " + string(gwy) + " dev " + string(dev);
    }
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
