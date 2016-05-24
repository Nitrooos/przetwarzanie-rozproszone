#ifndef MESSAGE_NEIGH_NEW_H
#define MESSAGE_NEIGH_NEW_H

#include "base.hpp"

namespace Message {

    namespace Neigh {
        
        class New : public Base {
            public:
                New(struct nlmsghdr *header);
                virtual string shout();
        };
        
    }
    
}

#endif
