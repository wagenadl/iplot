// Launcher.cpp

#include "Launcher.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

Launcher::Launcher(char const *command) {
  pid_ = 0;
  
  int svsend[2];
  if (socketpair(AF_UNIX, SOCK_STREAM, 0, svsend) < 0) {
    perror("Launcher");
    return;
  }
  
  int svrecv[2];
  if (socketpair(AF_UNIX, SOCK_STREAM, 0, svrecv) < 0) {
    perror("Launcher");
    shutdown(svsend[0], SHUT_RDWR);
    close(svsend[0]);
    close(svsend[1]);
    return;
  }

  pid_t p = fork();
  if (p<0) {
    perror("Launcher");
    shutdown(svsend[0], SHUT_RDWR);
    close(svsend[0]);
    close(svsend[1]);
    shutdown(svrecv[0], SHUT_RDWR);
    close(svrecv[0]);
    close(svrecv[1]);
    return;
  } else if (p==0) {
    // child
    setsid();
    int maxfd=sysconf(_SC_OPEN_MAX);
    for(int fd=0; fd<maxfd; fd++)
      if (fd!=svsend[0] && fd!=svrecv[0] && fd!=2)
	close(fd);
    dup2(svsend[0], 0);
    dup2(svrecv[0], 1);
    execlp(command, command, NULL);
    perror("Launcher");
    exit(1);
  }

  // parent
  close(svsend[0]);
  close(svrecv[0]);
  sfd = svsend[1];
  rfd = svrecv[1];
  pid_ = p;
  ok_ = true;
}  
  
Launcher::~Launcher() {
  if (!pid_)
    return;
  shutdown(sfd, SHUT_WR); // or should this be SHUT_RDWR?
  shutdown(rfd, SHUT_RDWR);
  close(sfd);
  close(rfd);
  int status;
  int p = waitpid(pid_, &status, 0);
  if (p<0) {
    perror("Launcher");
  } else {
    if (WIFEXITED(status)) {
      int exitstatus = WEXITSTATUS(status);
      if (exitstatus)
	fprintf(stderr, "Child exited with nonzero status: %i\n", exitstatus);
      else
	fprintf(stderr, "Child exited OK\n");
    } else if (WIFSIGNALED(status)) {
	fprintf(stderr, "Child exited upon signal: %i\n", WTERMSIG(status));
    } else {
      fprintf(stderr, "Child not exited (%i, 0x%x)\n", p, status);
    }
  }
}
