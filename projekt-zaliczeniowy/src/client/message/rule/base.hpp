#ifndef MESSAGE_RULE_BASE_H
#define MESSAGE_RULE_BASE_H

#include "../base.hpp"

namespace Message {
    
    namespace Rule {
        
        class Base : public Message::Base {
            public:
                Base(struct nlmsghdr *header);
                virtual string shout();
                static Rule::Base *build(struct nlmsghdr *header);
        };
        
    }
    
}

#endif
