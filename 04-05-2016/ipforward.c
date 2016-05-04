#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define IPPROTO_CUSTOM 222

void ip_send(char* data, char *dest) {
  int sfd;
  struct sockaddr_in addr;

  sfd = socket(PF_INET, SOCK_RAW, IPPROTO_CUSTOM);
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = 0;
  addr.sin_addr.s_addr = inet_addr(dest);
  sendto(sfd, data, strlen(data) + 1, 0, (struct sockaddr*) &addr, sizeof(addr));
  close(sfd);
}

char *ip_receive() {
  int sfd, rc;
  char buf[65536], saddr[16], daddr[16];
  char *data;
  socklen_t sl;
  struct sockaddr_in addr;
  struct iphdr *ip;

  sfd = socket(PF_INET, SOCK_RAW, IPPROTO_CUSTOM);
  while(1) {
    memset(&addr, 0, sizeof(addr));
    sl = sizeof(addr);
    rc = recvfrom(sfd, buf, sizeof(buf), 0, (struct sockaddr*) &addr, &sl);
    ip = (struct iphdr*) &buf;
    if (ip->protocol == IPPROTO_CUSTOM) {
      inet_ntop(AF_INET, &ip->saddr, (char*) &saddr, 16);
      inet_ntop(AF_INET, &ip->daddr, (char*) &daddr, 16);
      data = (char*)malloc(sizeof(struct iphdr));
      memcpy(data, (char*) ip + (ip->ihl * 4), sizeof(struct iphdr));
      printf("[%dB] %s -> %s | %s\n", rc - (ip->ihl * 4), saddr, daddr, data);
      break;
    }
  }
  close(sfd);
  return data;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Należy podać jako pierwszy argument adres docelowy do przesyłania pakietów IP\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    char *data = ip_receive();
    ip_send(data, argv[1]);
    free(data);
  }
}

