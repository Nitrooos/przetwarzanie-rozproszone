#ifndef MESSAGE_LINK_NEW_H
#define MESSAGE_LINK_NEW_H

#include "base.hpp"

namespace Message {

    namespace Link {
        
        class New : public Base {
            public:
                New(struct nlmsghdr *header);
                virtual string shout();
        };
        
    }
    
}

#endif
