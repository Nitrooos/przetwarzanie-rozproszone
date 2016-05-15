#include "new.hpp"
#include "../../exceptions/types.hpp"

#include <cstring>
#include <arpa/inet.h>

Message::Route::New::New(struct nlmsghdr *header) : Message::Route::Base(header) { }

string Message::Route::New::shout() {
    char dev[32] = "", dst[32] = "", msk[32] = "";
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

    return "[ROUTE ADD] " + string(dst) + "/" + string(msk) + " dev " + string(dev);
}
