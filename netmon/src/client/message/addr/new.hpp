#ifndef MESSAGE_ADDR_NEW_H
#define MESSAGE_ADDR_NEW_H

#include "base.hpp"

namespace Message {

    namespace Addr {
        
        class New : public Base {
            public:
                New(struct nlmsghdr *header);
                virtual string shout();
        };
        
    }
    
}

#endif
