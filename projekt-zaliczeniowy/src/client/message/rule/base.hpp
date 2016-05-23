#ifndef MESSAGE_RULE_BASE_H
#define MESSAGE_RULE_BASE_H

#include "../base.hpp"

namespace Message {
    
    namespace Rule {
        
        class Base : public Message::Base {
            string family(unsigned char family) const;
            string flags(unsigned int flags) const;
            string attributes() const;
            string get_table_name(unsigned char table) const;
            
            public:
                Base(struct nlmsghdr *header);
                virtual string shout();
                static Rule::Base *build(struct nlmsghdr *header);
        };
        
    }
    
}

#endif
