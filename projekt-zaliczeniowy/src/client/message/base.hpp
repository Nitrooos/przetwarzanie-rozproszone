#ifndef MESSAGE_BASE_H
#define MESSAGE_BASE_H

#include <list>
#include <string>
#include <linux/rtnetlink.h>

using namespace std;

namespace Message {
    
    class Base {
        protected:
            struct nlmsghdr *_header;
            list<struct rtattr*> _attributes;
            
            template <typename T> void set_attributes(T *message);
        public:
            Base(struct nlmsghdr *header);
            virtual string shout() = 0;
            static Base* build(struct nlmsghdr *header);
    };
    
}

template <typename T>
void Message::Base::set_attributes(T *message) {
    struct rtattr *atp = (struct rtattr*) RTM_RTA(message);
    int atlen = RTM_PAYLOAD(this->_header);
    for(;RTA_OK(atp, atlen); atp = RTA_NEXT(atp, atlen)) {
        this->_attributes.push_back(atp);
    }
}


#endif
