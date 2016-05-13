#ifndef DEL_H
#define DEL_H

#include "../base.hpp"

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
