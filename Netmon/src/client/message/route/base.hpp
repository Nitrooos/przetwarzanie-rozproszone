#ifndef MESSAGE_ROUTE_BASE_H
#define MESSAGE_ROUTE_BASE_H

#include "../base.hpp"

namespace Message {
    
    namespace Route {
        
        class Base : public Message::Base {
            struct AttributeValues {
                char _dst[32] = "",
                     _src[32] = "",
                     _dev[32] = "",
                     _gateway[32] = "";
            };
            
            string type(unsigned char type) const;
            string protocol(unsigned char protocol) const;
            string table(unsigned char table) const;
            string scope(unsigned char scope) const;
            string flags(unsigned int flags) const;
            string attributes(AttributeValues &attrs) const;
            AttributeValues read_attributes() const;
            
            public:
                Base(struct nlmsghdr *header);
                virtual string shout();
                static bool validate_header(struct nlmsghdr *header);
                static Route::Base *build(struct nlmsghdr *header);
        };
        
    }
    
}

#endif
