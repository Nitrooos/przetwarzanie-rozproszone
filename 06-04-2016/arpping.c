#include <pcap.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <linux/if_ether.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <libnet.h>

char* errbuf;
libnet_t *ln;
pcap_t* handle;

void stop() {
    libnet_destroy(ln);
    pcap_close(handle);
    free(errbuf);
}

struct arphdr {
    u_int16_t ftype;
    u_int16_t ptype;
    u_int8_t flen;
    u_int8_t plen;
    u_int16_t opcode;
    u_int8_t sender_mac_addr[6];
    u_int8_t sender_ip_addr[4];
    u_int8_t target_mac_addr[6];
    u_int8_t target_ip_addr[4];
};

int main(int argc, char** argv) {
    signal(SIGINT, stop);

    u_int32_t target_ip_addr, src_ip_addr;
    u_int8_t bcast_hw_addr[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
	     zero_hw_addr[6]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    struct libnet_ether_addr* src_hw_addr;
    char errbuf_ln[LIBNET_ERRBUF_SIZE];

    ln = libnet_init(LIBNET_LINK, NULL, errbuf_ln);
    src_ip_addr = libnet_get_ipaddr4(ln);
    src_hw_addr = libnet_get_hwaddr(ln);
    target_ip_addr = libnet_name2addr4(ln, argv[1], LIBNET_RESOLVE);
    libnet_autobuild_arp(
	ARPOP_REQUEST,                   /* operation type       */
	src_hw_addr->ether_addr_octet,   /* sender hardware addr */
	(u_int8_t*) &src_ip_addr,        /* sender protocol addr */
	zero_hw_addr,                    /* target hardware addr */
	(u_int8_t*) &target_ip_addr,     /* target protocol addr */
	ln);                             /* libnet context       */
    libnet_autobuild_ethernet(
	bcast_hw_addr,                   /* ethernet destination */
	ETHERTYPE_ARP,                   /* ethertype            */
	ln);                             /* libnet context       */

    struct pcap_pkthdr *pheader = NULL;
    struct ethhdr *fheader = NULL;
    struct arphdr *aheader = NULL;
    const u_char *frame = NULL;

    errbuf = malloc(PCAP_ERRBUF_SIZE);
    handle = pcap_create(libnet_getdevice(ln), errbuf);
    pcap_set_promisc(handle, 1);
    pcap_set_snaplen(handle, 65535);
    pcap_activate(handle);

    libnet_write(ln);
    printf("REQ %s\n", argv[1]);
    while (1) {
	int cap = pcap_next_ex(handle, &pheader, &frame);
	if (cap != 1) {
	    continue;
	}
	fheader = (struct ethhdr *)frame;
	if (ntohs(fheader->h_proto) != ETH_P_ARP) {
	    continue;
	}
	aheader = (struct arphdr *)(frame + ETH_HLEN);
	if (ntohs(aheader->opcode) == ARPOP_REPLY && memcmp(aheader->sender_ip_addr, (u_int8_t*)&target_ip_addr, 4) == 0) {
	    printf("REP %s | %02x:%02x:%02x:%02x:%02x:%02x\n", argv[1], aheader->sender_mac_addr[0], aheader->sender_mac_addr[1], aheader->sender_mac_addr[2],
	    		aheader->sender_mac_addr[3], aheader->sender_mac_addr[4], aheader->sender_mac_addr[5]);
	} else {
	    continue;
	}
    	libnet_write(ln);
	printf("REQ %s\n", argv[1]);
    }
}
