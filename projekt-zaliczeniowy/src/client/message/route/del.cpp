#include "del.hpp"

#include <arpa/inet.h>

Message::Route::Del::Del(struct nlmsghdr *header) : Message::Route::Base(header) { }

string Message::Route::Del::shout() {
    char dev[32], dst[32], msk[32];
    for (auto &attr : this->_attributes) {
        switch(attr->rta_type) {
            case RTA_DST:
                inet_ntop(AF_INET, RTA_DATA(attr), dst, sizeof(dst));
                break;
            case RTA_OIF:
                sprintf(dev, "%d", *((int*) RTA_DATA(attr)));
                break;
        }
    }
    
    sprintf(msk, "%d", ((struct rtmsg*)NLMSG_DATA(this->_header))->rtm_dst_len);

    return "[ROUTE DEL] " + string(dst) + "/" + string(msk) + " dev " + string(dev);
}
