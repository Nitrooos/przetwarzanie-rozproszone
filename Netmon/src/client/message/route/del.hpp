#ifndef MESSAGE_ROUTE_DEL_H
#define MESSAGE_ROUTE_DEL_H

#include "base.hpp"

namespace Message {

    namespace Route {
        
        class Del : public Base {
            public:
                Del(struct nlmsghdr *header);
                virtual string shout();
        };
        
    }
    
}

#endif
