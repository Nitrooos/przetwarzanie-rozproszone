#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int sfd, cfd;
    socklen_t sl;
    struct sockaddr_in saddr, caddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(1234);
    sfd = socket(PF_INET, SOCK_STREAM, 0);

    int nFoo = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char*)&nFoo, sizeof(nFoo));

    bind(sfd, (struct sockaddr*)&saddr, sizeof(saddr));
    listen(sfd, 5);

    char buf[128] = "";
    int client_sfd;
    struct sockaddr_in client_saddr;
    struct hostent* addrent;
    addrent = gethostbyname(argv[1]);
    memset(&client_saddr, 0, sizeof(client_saddr));
    client_saddr.sin_family = AF_INET;
    client_saddr.sin_port = htons(atoi(argv[2]));
    memcpy(&client_saddr.sin_addr.s_addr, addrent->h_addr, addrent->h_length);
    client_sfd = socket(PF_INET, SOCK_STREAM, 0);
    connect(client_sfd, (struct sockaddr*)&client_saddr, sizeof(client_saddr));

    while(1) {
        sl = sizeof(caddr);
        cfd = accept(sfd, (struct sockaddr*)&caddr, &sl);
        read(cfd, buf, 128);
        close(cfd);

        write(1, buf, 128);
        write(client_sfd, buf, 128);
    }
}
