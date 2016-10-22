#ifndef MESSAGE_LINK_DEL_H
#define MESSAGE_LINK_DEL_H

#include "base.hpp"

namespace Message {

    namespace Link {
        
        class Del : public Base {
            public:
                Del(struct nlmsghdr *header);
                virtual string shout();
        };
        
    }
    
}

#endif
