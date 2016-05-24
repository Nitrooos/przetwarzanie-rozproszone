#ifndef MESSAGE_RULE_DEL_H
#define MESSAGE_RULE_DEL_H

#include "base.hpp"

namespace Message {

    namespace Rule {
        
        class Del : public Base {
            public:
                Del(struct nlmsghdr *header);
                virtual string shout();
        };
        
    }
    
}

#endif
