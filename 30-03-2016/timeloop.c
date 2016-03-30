#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>

#define DURATION 30 /* seconds */

struct connection {
    int descriptior;
    struct sockaddr_in address;
};

struct connection *conn_new(uint32_t address, int port) {
    struct connection *ptr = (struct connection*)malloc(sizeof(struct connection));
    ptr->descriptior = socket(PF_INET, SOCK_DGRAM, 0);
    
    memset(&ptr->address, 0, sizeof(ptr->address));
    ptr->address.sin_family = AF_INET;
    ptr->address.sin_addr.s_addr = address;
    ptr->address.sin_port = htons(port);

    bind(ptr->descriptior, (struct sockaddr*)&ptr->address, sizeof(ptr->address));

    return ptr;
}

struct connection *outcoming;

void interrupt_handler(int signal) {
    close(outcoming->descriptior);
    free(outcoming);
    printf("\nPoprawnie zwolniono zasoby, zakończenie programu...\n");
    exit(0);
}

void interrupt_bind_handler() {
    struct sigaction interrupt;
    interrupt.sa_handler = interrupt_handler;
    sigemptyset(&interrupt.sa_mask);
    interrupt.sa_flags = 0;

    sigaction(SIGINT, &interrupt, NULL);
}

int main(int argc, char** argv) {
    interrupt_bind_handler();

    struct hostent *dest_host = gethostbyname(argv[1]);
    uint32_t dest_addr;
    memcpy(&dest_addr, dest_host->h_addr, dest_host->h_length);

    outcoming = conn_new(dest_addr, atoi(argv[2]));

    char buf[1100] = "";

	time_t start = time(NULL), stop;

    unsigned long long sent = 0;
	printf("start @ %s", ctime(&start));
	while(time(NULL) < start + DURATION) {
        memset(&buf, '-', sizeof(buf));
        sent += sendto(outcoming->descriptior, buf, strlen(buf) + 1, 0,
               (struct sockaddr*)&outcoming->address, sizeof(outcoming->address));
	}
	stop = time(NULL);
	printf("stop  @ %s\n", ctime(&stop));
    printf("wysłano %lld bajtów\n", sent);
	return EXIT_SUCCESS;
}

