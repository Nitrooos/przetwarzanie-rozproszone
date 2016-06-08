#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

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

void *thread_mainloop(void *args) {
  int *cfd = (int*) args;
  int i;
  char buf[4096];

  memset(&buf, 0, sizeof(buf));
  _read(*cfd, buf, 256);
  for (i = 0; i < 4096; i++) buf[i] = i;
  _write(*cfd, buf, 4096);
  close(*cfd);
}

int main(int argc, char** argv) {
  long double n = 0;
  int sfd, cfd, on = 1, i;
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

    pthread_t thread;
    int t = pthread_create(&thread, NULL, thread_mainloop, (void*) &cfd);
    pthread_detach(thread);

    n += 1;
    printf("%.0Lf\n", n);
  }
  close(sfd);
  return EXIT_SUCCESS;
}
