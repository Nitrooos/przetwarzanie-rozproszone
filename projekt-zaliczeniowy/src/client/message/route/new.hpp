#ifndef MESSAGE_ROUTE_NEW_H
#define MESSAGE_ROUTE_NEW_H

#include "base.hpp"

namespace Message {

    namespace Route {
        
        class New : public Base {
            public:
                New(struct nlmsghdr *header);
                virtual string shout();
        };
        
    }
    
}

#endif
