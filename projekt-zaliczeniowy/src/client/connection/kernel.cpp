#include "kernel.hpp"
#include "../exceptions/types.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <iostream>

Connection::Kernel::Kernel() {
    if ((this->_socket = socket(PF_NETLINK, SOCK_RAW, NETLINK_ROUTE)) < 0) {
        throw Exception::Error("Connection::Kernel socket(): cannot create socket");
    }
    
    struct sockaddr_nl sock_addr = this->prepare_sockaddr();
    
    if (bind(this->_socket, (struct sockaddr*) &sock_addr, sizeof sock_addr) < 0) {
        throw Exception::Error("Connection::Kernel bind(): there's no connection between process and kernel");
    }
}

struct sockaddr_nl Connection::Kernel::prepare_sockaddr() {
    struct sockaddr_nl sock_addr;

    memset(&sock_addr, 0, sizeof sock_addr);
    sock_addr.nl_family = AF_NETLINK;
    sock_addr.nl_pid = getpid();
    sock_addr.nl_groups = RTMGRP_IPV4_ROUTE;
    
    return sock_addr;
}

list<Message::Base*> Connection::Kernel::receive() {
    list<Message::Base*> messages;
    
    int nl_length;//, atlen;
    char buf[8192];
    //char dst[32], msk[32], gwy[32], dev[32];
    struct nlmsghdr *nl_message;
    struct rtmsg *rtp;
    //struct rtattr *atp;
    
    nl_length = this->receive_msg_headers(buf, sizeof buf);
    nl_message = (struct nlmsghdr*) buf;
    for(;NLMSG_OK(nl_message, nl_length); nl_message = NLMSG_NEXT(nl_message, nl_length)) {
        rtp = (struct rtmsg*) NLMSG_DATA(nl_message);
        if (rtp->rtm_table != RT_TABLE_MAIN) {
            continue;
        }
        messages.push_back(Message::Base::build(nl_message));
    }
    
    return messages;
    
    //~nl_message = (struct nlmsghdr*) buf;
    //~for(;NLMSG_OK(nl_message, nl_length); nl_message = NLMSG_NEXT(nl_message, nl_length)) {
        //~rtp = (struct rtmsg*) NLMSG_DATA(nl_message);
        //~if (rtp->rtm_table != RT_TABLE_MAIN) {
            //~continue;
        //~}
        //~atp = (struct rtattr*) RTM_RTA(rtp);
        //~atlen = RTM_PAYLOAD(nl_message);
        
        //~memset(dst, 0, sizeof(dst));
        //~memset(msk, 0, sizeof(msk));
        //~memset(gwy, 0, sizeof(gwy));
        //~memset(dev, 0, sizeof(dev));
        //~for(;RTA_OK(atp, atlen); atp = RTA_NEXT(atp, atlen)) {
            //~switch(atp->rta_type) {
                //~case RTA_DST:
                    //~inet_ntop(AF_INET, RTA_DATA(atp), dst, sizeof(dst));
                    //~break;
                //~case RTA_GATEWAY:
                    //~inet_ntop(AF_INET, RTA_DATA(atp), gwy, sizeof(gwy));
                    //~break;
                //~case RTA_OIF:
                    //~sprintf(dev, "%d", *((int*) RTA_DATA(atp)));
                    //~break;
            //~}
        //~}
        //~sprintf(msk, "%d", rtp->rtm_dst_len);
        //~if (nl_message->nlmsg_type == RTM_DELROUTE) {
            //~cout << "[DEL] ";
        //~} else if (nl_message->nlmsg_type == RTM_NEWROUTE) {
            //~cout << "[ADD] ";
        //~}
        
        //~if (strlen(dst) == 0) {
            //~cout << "default via " << gwy << " dev " << dev << "\n";
        //~} else if (strlen(gwy) == 0) {
            //~cout << dst << "/" << msk << " dev " << dev << "\n";
        //~} else {
            //~cout << "dst " << dst << "/" << msk << " gwy " << gwy << " dev " << dev << "\n";
        //~}
    //~}
}

int Connection::Kernel::receive_msg_headers(char *buf, int buf_len) {
    char *ptr;
    int length;
    struct nlmsghdr *nl_msg_header;
    
    ptr = buf;
    length = 0;
    
    /// ten blok ma na celu ustalenie wartości nllen 
    do {
        /// czekanie na wiadomość
        int received_bytes = recv(this->_socket, ptr, buf_len - length, 0);
        /// pobranie headera wiadomości
        nl_msg_header = (struct nlmsghdr*) ptr;
        ptr += received_bytes;
        length += received_bytes;
        /// aż do odebrania wszystkich nagłówków
    } while (nl_msg_header->nlmsg_type == NLMSG_DONE);
    
    return length;
}
