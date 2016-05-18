#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define LOCAL_TIME_STREAM 0
#define GREENWICH_MEAN_TIME_STREAM 1

int main(int argc, char **argv) {
  int cfd, flags, port, i, rc, no = argc - 3;
  socklen_t sl = sizeof(struct sockaddr_in);
  struct sockaddr_in addr, *addrs, saddr;
  struct sctp_sndrcvinfo sndrcvinfo;
  struct sctp_event_subscribe events;
  char buf[1024];

  cfd = socket(PF_INET, SOCK_STREAM, IPPROTO_SCTP);

  addrs = malloc(sl * no);
  memset(&addr, 0, sl);
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(argv[2]);
  addr.sin_port = 0;
  bind(cfd, (struct sockaddr*) &addr, sl);
  getsockname(cfd, (struct sockaddr*) &addr, &sl);
  port = addr.sin_port;
  for(i = 3; i < argc; i++) {
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[i]);
    addr.sin_port = port;
    memcpy(addrs + (i-3), &addr, sl);
  }
  sctp_bindx(cfd, (struct sockaddr*) addrs, no, SCTP_BINDX_ADD_ADDR);
  no = sctp_getladdrs(cfd, 0, (struct sockaddr**) &addrs);
  for (i = 0; i < no; i++) {
    memcpy(&addr, addrs + i, sl);
    printf("%d: %s:%d\n", i, inet_ntoa(addr.sin_addr), addr.sin_port);
  }

  for (i = 1; i < argc - no; ++i) {
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(1234);
    saddr.sin_addr.s_addr = inet_addr(argv[i]);
    memcpy(addrs + (i-1), &saddr, sl);
  }
  sctp_connectx(cfd, (struct sockaddr*) addrs, no, NULL);

  memset((void*) &events, 0, sizeof(events));
  events.sctp_data_io_event = 1;
  setsockopt(cfd, SOL_SCTP, SCTP_EVENTS, (const void*) &events,
             sizeof(events));
  while (1) {
    flags = 0;
    rc = sctp_recvmsg(cfd, (void*) buf, sizeof(buf), (struct sockaddr*) NULL,
                      0, &sndrcvinfo, &flags);
    buf[rc] = 0;
    if (sndrcvinfo.sinfo_stream == LOCAL_TIME_STREAM) {
      printf("(Local) %s", buf);
    } else if (sndrcvinfo.sinfo_stream == GREENWICH_MEAN_TIME_STREAM) {
      printf("(GMT)   %s", buf);
    }
  }
  close(cfd);
  return EXIT_SUCCESS;
}
