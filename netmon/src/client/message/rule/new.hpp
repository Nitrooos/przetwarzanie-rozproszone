#ifndef MESSAGE_RULE_NEW_H
#define MESSAGE_RULE_NEW_H

#include "base.hpp"

namespace Message {

    namespace Rule {
        
        class New : public Base {
            public:
                New(struct nlmsghdr *header);
                virtual string shout();
        };
        
    }
    
}

#endif
