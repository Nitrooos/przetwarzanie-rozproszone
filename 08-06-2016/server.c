/*
 * Copyright (C) 2016 Michal Kalewski <mkalewski at cs.put.poznan.pl>
 *
 * Compilation:  gcc -Wall ./server.c -o ./server
 * Usage:        ./server
 *
 * Bug reports:  https://gitlab.cs.put.poznan.pl/mkalewski/ps-2016/issues
 *
 */

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

struct req_args {
    int socket_no;
    long double n;
};

int _read(int fd, char *buf, int len) {
  int rc, total = 0;
  while (len > 0) {
    rc = read(fd, buf, len);
    len -= rc;
    buf += rc;
    total += rc;
  }
  return total;
}

int _write(int fd, char *buf, int len){
  int rc, total = 0;
  while (len > 0) {
    rc = write(fd, buf, len);
    len -= rc;
    buf += rc;
    total += rc;
  }
  return total;
}

void *request_handle(void *args) {
    struct req_args *arguments = (struct req_args*)args;

    char buf[4096];
    int i;
    memset(&buf, 0, sizeof(buf));
    _read(arguments->socket_no, buf, 256);
    for (i = 0; i < 4096; i++) buf[i] = i;
    _write(arguments->socket_no, buf, 4096);
    arguments->n += 1;

    close(arguments->socket_no);
    //pthread_exit(NULL);
}

int main() {
  long double n = 0;
  int sfd, cfd, on = 1;
  socklen_t sl;
  struct sockaddr_in saddr, caddr;

  memset(&saddr, 0, sizeof(saddr));
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = INADDR_ANY;
  saddr.sin_port = htons(1234);
  sfd = socket(PF_INET, SOCK_STREAM, 0);
  setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char*) &on, sizeof(on));
  bind(sfd, (struct sockaddr*) &saddr, sizeof(saddr));
  listen(sfd, 100);
  while(1) {
    memset(&caddr, 0, sizeof(caddr));
    sl = sizeof(caddr);
    cfd = accept(sfd, (struct sockaddr*) &caddr, &sl);
    
    struct req_args args;
    args.n = n;
    args.socket_no = cfd;

    pthread_t request;
    if (pthread_create(&request, NULL, request_handle, (void*)&args)) {
        printf("Nie udało się utworzyć wątku dla obsługi żądania\n");
        exit(EXIT_FAILURE);
    }
    pthread_detach(request);
    //pthread_join(request, NULL);

    n += 1;
  }
  close(sfd);
  return EXIT_SUCCESS;
}
