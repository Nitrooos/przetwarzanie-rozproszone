#ifndef BASE_H
#define BASE_H

#include <list>
#include <string>
#include <linux/rtnetlink.h>

using namespace std;

namespace Message {
    
    class Base {
        struct nlmsghdr _header;
        struct rtmsg _rtnetlink_message;
        list<struct rtattr> _attributes;

        public:
            Base(struct nlmsghdr *header);
            virtual string shout() = 0;
            static Base* build(struct nlmsghdr *header);
    };
    
}

#endif
