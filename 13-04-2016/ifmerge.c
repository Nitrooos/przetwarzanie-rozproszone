#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/if.h>
#include <linux/route.h>
#include <linux/if_arp.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <sys/ioctl.h>

#define IRI_T_ADDRESS 0
#define IRI_T_ROUTE   1

#define ETH_P_CUSTOM 0x8888

struct ifrtinfo {
    int iri_type;
    char iri_iname[16];
    struct sockaddr_in iri_iaddr;
    struct sockaddr_in iri_rtdst;
    struct sockaddr_in iri_rtmsk;
    struct sockaddr_in iri_rtgip;
};

void ifsetup(char *ifname, uint32_t ip) {
    int fd;
    struct ifreq ifr;
    struct sockaddr_in* sin;

    fd = socket(PF_INET, SOCK_DGRAM, 0);
    strncpy(ifr.ifr_name, ifname, strlen(ifname) + 1);
    sin = (struct sockaddr_in*) &ifr.ifr_addr;
    memset(sin, 0, sizeof(struct sockaddr_in));
    sin->sin_family = AF_INET;
    sin->sin_port = 0;
    sin->sin_addr.s_addr = ip;
    ioctl(fd, SIOCSIFADDR, &ifr);
    ioctl(fd, SIOCGIFFLAGS, &ifr);
    ifr.ifr_flags |= IFF_UP | IFF_RUNNING;
    ioctl(fd, SIOCSIFFLAGS, &ifr);
    close(fd);
}

void set_gateway(uint32_t gateway, uint32_t dst_ip, uint32_t dst_netmask) {
    int fd;
    struct rtentry route;
    struct sockaddr_in* addr;

    fd = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&route, 0, sizeof(route));
    addr = (struct sockaddr_in*) &route.rt_gateway;
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = gateway;
    addr = (struct sockaddr_in*) &route.rt_dst;
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = dst_ip;
    addr = (struct sockaddr_in*) &route.rt_genmask;
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = dst_netmask;
    route.rt_flags = RTF_UP;
    route.rt_metric = 0;
    ioctl(fd, SIOCADDRT, &route);
    close(fd);
}

int main(int argc, char *argv[]) {
    int sfd, i;
    ssize_t len;
    char* frame;
    char* fdata;
    struct ethhdr* fhead;
    struct ifreq ifr;
    struct sockaddr_ll sall;

    sfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_CUSTOM));
    strncpy(ifr.ifr_name, argv[1], IFNAMSIZ);
    ioctl(sfd, SIOCGIFINDEX, &ifr);
    memset(&sall, 0, sizeof(struct sockaddr_ll));
    sall.sll_family = AF_PACKET;
    sall.sll_protocol = htons(ETH_P_CUSTOM);
    sall.sll_ifindex = ifr.ifr_ifindex;
    sall.sll_hatype = ARPHRD_ETHER;
    sall.sll_pkttype = PACKET_HOST;
    sall.sll_halen = ETH_ALEN;
    bind(sfd, (struct sockaddr*) &sall, sizeof(struct sockaddr_ll));
    while(1) {
        frame = malloc(ETH_FRAME_LEN);
        memset(frame, 0, ETH_FRAME_LEN);
        fhead = (struct ethhdr*) frame;
        fdata = frame + ETH_HLEN;
        len = recvfrom(sfd, frame, ETH_FRAME_LEN, 0, NULL, NULL);
        /*printf("[%dB] %02x:%02x:%02x:%02x:%02x:%02x -> ", (int)len,
               fhead->h_source[0], fhead->h_source[1], fhead->h_source[2],
               fhead->h_source[3], fhead->h_source[4], fhead->h_source[5]);
        printf("%02x:%02x:%02x:%02x:%02x:%02x | ",
               fhead->h_dest[0], fhead->h_dest[1], fhead->h_dest[2],
               fhead->h_dest[3], fhead->h_dest[4], fhead->h_dest[5]);
        printf("%s\n", fdata);
        for (i = 0; i < len ; i++) {
            printf("%02x ", (unsigned char) frame[i]);
            if ((i + 1) % 16 == 0)
                printf("\n");
        }
        printf("\n\n");*/

        struct ifrtinfo *request = (struct ifrtinfo*) fdata;
        switch (request->iri_type) {
            case IRI_T_ADDRESS:
                ifsetup(request->iri_iname, request->iri_iaddr.sin_addr.s_addr);
                break;
            case IRI_T_ROUTE:
                set_gateway(request->iri_rtgip.sin_addr.s_addr, request->iri_rtdst.sin_addr.s_addr, request->iri_rtmsk.sin_addr.s_addr);
                break;
            default:
                printf("Otrzymano request z nieznanym typem operacji!\n");
                break;
        }

        free(frame);
    }
    close(sfd);
    return EXIT_SUCCESS;
}
