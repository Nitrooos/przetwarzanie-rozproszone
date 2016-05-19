#ifndef MESSAGE_NEIGH_BASE_H
#define MESSAGE_NEIGH_BASE_H

#include "../base.hpp"

namespace Message {
    
    namespace Neigh {
        
        class Base : public Message::Base {
            string state(__u16 state) const;
            string flags(__u8  flags) const;
            string attributes() const;
            string make_hwaddr_string(unsigned char *hwaddr) const;
            
            public:
                Base(struct nlmsghdr *header);
                virtual string shout();
                static Neigh::Base *build(struct nlmsghdr *header);
        };
        
    }
    
}

#endif
