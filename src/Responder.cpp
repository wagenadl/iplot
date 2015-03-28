// Responder.cpp

#include "Responder.h"
#include <sys/socket.h>
#include <unistd.h>

Responder::Responder() {
  rfd = 0;
  sfd = 1;
  ok_ = true;
}

Responder::~Responder() {
  shutdown(sfd, SHUT_WR); // or should this be SHUT_RDWR?
  shutdown(rfd, SHUT_RD);
  close(sfd);
  close(rfd);
}
