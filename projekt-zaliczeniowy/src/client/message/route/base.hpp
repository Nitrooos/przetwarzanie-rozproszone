#ifndef MESSAGE_ROUTE_BASE_H
#define MESSAGE_ROUTE_BASE_H

#include "../base.hpp"

namespace Message {
    
    namespace Route {
        
        class Base : public Message::Base {
            public:
                Base(struct nlmsghdr *header);
                virtual string shout();
                static bool validate_header(struct nlmsghdr *header);
                static Route::Base *build(struct nlmsghdr *header);
        };
        
    }
    
}

#endif
