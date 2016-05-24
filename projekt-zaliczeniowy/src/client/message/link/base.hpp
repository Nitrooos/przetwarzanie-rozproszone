#ifndef MESSAGE_LINK_BASE_H
#define MESSAGE_LINK_BASE_H

#include "../base.hpp"

namespace Message {
    
    namespace Link {
        
        class Base : public Message::Base {
            string flags(unsigned int flags) const;
            string attributes() const;
            
            public:
                Base(struct nlmsghdr *header);
                virtual string shout();
                static Link::Base *build(struct nlmsghdr *header);
        };
        
    }
    
}

#endif
