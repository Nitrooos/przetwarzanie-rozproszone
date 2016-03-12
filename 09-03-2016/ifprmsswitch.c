#include <arpa/inet.h>
#include <errno.h>
#include <net/if.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int sfd;
    struct ifreq ifr;
    char *on_or_off = (char*)malloc(100);

    strncpy(on_or_off, argv[2], 100);

    // utwórz socket dla funkcji ioctl i przypisz nazwę interfejsu na podstawie pierwszego argumentu
    sfd = socket(PF_INET, SOCK_DGRAM, 0);
    strncpy(ifr.ifr_name, argv[1], IFNAMSIZ);
    
    // pobierz flagi interfejsu sieciowego
    ioctl(sfd, SIOCGIFFLAGS, &ifr);
    // wykonaj odpowiednią akcję na podstawie drugiego argumentu
    if (strcmp(on_or_off, "on") == 0) {
        ifr.ifr_flags |= IFF_PROMISC;
    } else if (strcmp(on_or_off, "off") == 0) {
        ifr.ifr_flags &= ~IFF_PROMISC;
    } else {
        // wyświetl komunikat o błędzie
        printf("Invalid option: %s\nUse 'on' or 'off' instead\n", on_or_off);
        free(on_or_off);
        exit(1);
    }

    // ustaw odpowiednie flagi (włączenie lub wyłączenie trybu nasłuchiwania PROMISCOUS)
    ioctl(sfd, SIOCSIFFLAGS, &ifr);

    // sprzątanie
    free(on_or_off);
}
