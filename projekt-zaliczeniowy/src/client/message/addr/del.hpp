#ifndef MESSAGE_ADDR_DEL_H
#define MESSAGE_ADDR_DEL_H

#include "base.hpp"

namespace Message {

    namespace Addr {
        
        class Del : public Base {
            public:
                Del(struct nlmsghdr *header);
                virtual string shout();
        };
        
    }
    
}

#endif
