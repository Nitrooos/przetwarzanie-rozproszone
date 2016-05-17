#ifndef MESSAGE_LINK_BASE_H
#define MESSAGE_LINK_BASE_H

#include "../base.hpp"

namespace Message {
    
    namespace Link {
        
        class Base : public Message::Base {
            public:
                Base(struct nlmsghdr *header);
                virtual string shout();
                static Link::Base *build(struct nlmsghdr *header);
        };
        
    }
    
}

#endif
