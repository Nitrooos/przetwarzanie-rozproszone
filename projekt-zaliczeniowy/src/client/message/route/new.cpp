#include "new.hpp"

#include <cstring>
#include <arpa/inet.h>

Message::Route::New::New(struct nlmsghdr *header) : Message::Route::Base(header) { }

string Message::Route::New::shout() {
    char dev[32], dst[32], msk[32];
    memset(dev, 0, sizeof(dev));
    memset(dst, 0, sizeof(dst));
    memset(msk, 0, sizeof(msk));
    
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

    return "[ROUTE ADD] " + string(dst) + "/" + string(msk) + " dev " + string(dev);
}
