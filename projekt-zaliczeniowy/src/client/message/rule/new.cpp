#include "new.hpp"

Message::Rule::New::New(struct nlmsghdr *header) : Message::Rule::Base(header) { }

string Message::Rule::New::shout() {
    return "RULE ADD " + Rule::Base::shout();
}
