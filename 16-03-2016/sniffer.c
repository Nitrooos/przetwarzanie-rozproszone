#include <arpa/inet.h>
#include <linux/if_arp.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

int sfd, i, ifindex;
ssize_t len;
char* frame;
char* fdata;
struct ethhdr* fhead;
struct ifreq ifr;
struct sockaddr_ll sall;

char *get_packet_type(int type) {
    char *packet_type = (char*)malloc(100);
    switch (type) {
        case 0: strcpy(packet_type,  "HOST");      break;
        case 1: strcpy(packet_type,  "BROADCAST"); break;
        case 2: strcpy(packet_type,  "MULTICAST"); break;
        case 3: strcpy(packet_type,  "OTHERHOST"); break;
        case 4: strcpy(packet_type,  "OUTGOING");  break;
        default: strcpy(packet_type, "UNKNOWN");   break;
    }
    return packet_type;
}

char *get_packet_net_proto(unsigned short proto) {
    char *net_proto = (char*)malloc(100);
    switch (proto) {
        case 0x0800: strcpy(net_proto, "IPv4");  break;
        case 0x86DD: strcpy(net_proto, "IPv6");  break;
        case 0x0806: strcpy(net_proto, "ARP");   break;
        default: strcpy(net_proto, "UNKNOWN"); break;
    }
    return net_proto;
}

void listen_frame(char *interface) {
	sfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	strncpy(ifr.ifr_name, interface, IFNAMSIZ);
	ioctl(sfd, SIOCGIFINDEX, &ifr);
	memset(&sall, 0, sizeof(struct sockaddr_ll));
	sall.sll_family = AF_PACKET;
	sall.sll_protocol = htons(ETH_P_ALL);
	sall.sll_ifindex = ifr.ifr_ifindex;
	sall.sll_hatype = ARPHRD_ETHER;
	sall.sll_pkttype = PACKET_HOST;
	sall.sll_halen = ETH_ALEN;
	bind(sfd, (struct sockaddr*) &sall, sizeof(struct sockaddr_ll));

	frame = malloc(ETH_FRAME_LEN);
	memset(frame, 0, ETH_FRAME_LEN);
	fhead = (struct ethhdr*) frame;
	fdata = frame + ETH_HLEN;
	len = recvfrom(sfd, frame, ETH_FRAME_LEN, 0, NULL, NULL);
	printf("[%dB] %02x:%02x:%02x:%02x:%02x:%02x -> ", (int)len,
	   fhead->h_source[0], fhead->h_source[1], fhead->h_source[2],
	   fhead->h_source[3], fhead->h_source[4], fhead->h_source[5]);
	printf("%02x:%02x:%02x:%02x:%02x:%02x | ",
	   fhead->h_dest[0], fhead->h_dest[1], fhead->h_dest[2],
	   fhead->h_dest[3], fhead->h_dest[4], fhead->h_dest[5]);

    char *packet_type = get_packet_type(sall.sll_pkttype);
    printf("%s | ", packet_type);
    free(packet_type);

    char *net_proto = get_packet_net_proto(ntohs(fhead->h_proto));
    printf("%s\n", net_proto);
    free(net_proto);

	for (i = 0; i < len ; i++) {
		printf("%02x ", (unsigned char) frame[i]);
		if ((i + 1) % 16 == 0)
			printf("\n");
	}
	printf("\n\n");
	free(frame);

	close(sfd);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        perror("Za mało argumentów! Użycie: ./sniffer <interfejs_sieciowy>\nNp. ./sniffer eth0\n");
    }

	while(1) {
		listen_frame(argv[1]);
	}
}
