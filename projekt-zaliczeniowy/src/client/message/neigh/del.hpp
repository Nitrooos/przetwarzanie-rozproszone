#ifndef MESSAGE_NEIGH_DEL_H
#define MESSAGE_NEIGH_DEL_H

#include "base.hpp"

namespace Message {

    namespace Neigh {
        
        class Del : public Base {
            public:
                Del(struct nlmsghdr *header);
                virtual string shout();
        };
        
    }
    
}

#endif
