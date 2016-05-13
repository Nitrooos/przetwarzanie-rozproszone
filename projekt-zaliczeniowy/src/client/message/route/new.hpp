#ifndef NEW_H
#define NEW_H

#include "../base.hpp"

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
