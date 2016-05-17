#ifndef MESSAGE_ADDR_BASE_H
#define MESSAGE_ADDR_BASE_H

#include "../base.hpp"

namespace Message {
    
    namespace Addr {
        
        class Base : public Message::Base {
            public:
                Base(struct nlmsghdr *header);
                virtual string shout();
                static Addr::Base *build(struct nlmsghdr *header);
        };
        
    }
    
}

#endif
