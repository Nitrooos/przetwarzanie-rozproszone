#include <pcap.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <linux/if_ether.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#define IP_P_TCP 0x06
#define IP_P_UDP 0x11

char* errbuf;
pcap_t* handle;

struct statistic {
    int _arp, _ip, _ip_with_udp, _ip_with_tcp, _other;
} global_statistic = { 0, 0, 0, 0, 0 };

void cleanup() {
    pcap_close(handle);
    free(errbuf);
}

void stop(int signo) {
    printf("Zebrane statystyki:\nARP: %5d | IP: %5d | IP/TCP: %5d | IP/UDP: %5d | Other: %5d\n", 
            global_statistic._arp, global_statistic._ip, global_statistic._ip_with_tcp, global_statistic._ip_with_udp, global_statistic._other);
    exit(EXIT_SUCCESS);
}

void trap(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
    int classified = 0;

    struct ethhdr *e_head = (struct ethhdr*)bytes;
    unsigned short net_proto = ntohs(e_head->h_proto);
    if (net_proto == ETH_P_ARP) {
        ++global_statistic._arp;
        classified = 1;
    } else if (net_proto == ETH_P_IP) {
        ++global_statistic._ip;
        classified = 1;
    }

    struct iphdr *ip_head = (struct iphdr*)(bytes + ETH_HLEN);
    unsigned short ip_proto = ntohs(ip_head->protocol) >> 8;
    if (ip_proto == IP_P_TCP) {
        ++global_statistic._ip_with_tcp;
        classified = 1;
    } else if (ip_proto == IP_P_UDP) {
        ++global_statistic._ip_with_udp;
        classified = 1;
    } else if (!classified) {
        ++global_statistic._other;
    }
}

int main(int argc, char** argv) {
    bpf_u_int32 netp, maskp;
    struct bpf_program fp;

    atexit(cleanup);
    signal(SIGINT, stop);

    errbuf = malloc(PCAP_ERRBUF_SIZE);
    handle = pcap_create(argv[1], errbuf);
    pcap_set_promisc(handle, 1);
    pcap_set_snaplen(handle, 65535);
    pcap_activate(handle);
    pcap_lookupnet(argv[1], &netp, &maskp, errbuf);
    pcap_compile(handle, &fp, "arp or ip or tcp or udp", 0, netp);
    if (pcap_setfilter(handle, &fp) < 0) {
        pcap_perror(handle, "pcap_setfilter()");
        exit(EXIT_FAILURE);
    }
    pcap_loop(handle, -1, trap, NULL);
}
