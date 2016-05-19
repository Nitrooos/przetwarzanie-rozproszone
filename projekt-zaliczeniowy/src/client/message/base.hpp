#ifndef MESSAGE_BASE_H
#define MESSAGE_BASE_H

#include <list>
#include <string>
#include <linux/rtnetlink.h>

using namespace std;

namespace Message {
    
    class Base {
        protected:
            struct nlmsghdr *_header;
            list<struct rtattr*> _attributes;
            
            string device(int device) const;
            
            void print(ostream & S, string header, int indent = 1) const;
            void print(ostream & S, string flag_as_string, string value, int indent = 1) const;
            void print(ostream & S, int state, int flag, string flag_as_string, string info) const;
            void set_attributes(struct rtattr *attrs);
        public:
            Base(struct nlmsghdr *header);
            virtual string shout() = 0;
            static Base* build(struct nlmsghdr *header);
    };
    
}

#endif
