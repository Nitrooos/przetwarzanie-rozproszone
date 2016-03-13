#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>

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

struct connection *incoming, *outcoming;

void interrupt_handler(int signal) {
    close(incoming->descriptior);
    close(outcoming->descriptior);
    free(incoming);
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

int main(int argc, char *argv[]) {
    interrupt_bind_handler();

    struct hostent *dest_host = gethostbyname(argv[1]);
    uint32_t dest_addr;
    memcpy(&dest_addr, dest_host->h_addr, dest_host->h_length);

    incoming = conn_new(INADDR_ANY, 1234);
    outcoming = conn_new(dest_addr, atoi(argv[2]));

    char buf[128] = "";
    while(1) {
        memset(&buf, 0, sizeof(buf));

        recvfrom(incoming->descriptior, buf, 128, 0, NULL, NULL);
        printf("Otrzymano wiadomość: %s na porcie %d\n", buf, (uint16_t)incoming->address.sin_port);

        sendto(outcoming->descriptior, buf, strlen(buf) + 1, 0,
               (struct sockaddr*)&outcoming->address, sizeof(outcoming->address));
        printf("Wysłano wiadomość: %s pod adres %s na port %d\n", buf, 
                inet_ntoa((struct in_addr)outcoming->address.sin_addr), (uint16_t)outcoming->address.sin_port);
    }
}
