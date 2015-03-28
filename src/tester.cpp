// tester.cpp

// g++ -W -Wall tester.cpp Launcher.cpp LRBase.cpp -o tester

#include "Launcher.h"
#include "Command.h"
#include <stdio.h>
#include <unistd.h>

int run() {
  Launcher lau("/home/wagenaar/progs/iplot/trunk/iplot_debug");
  Launcher::Header hdr;
  Launcher::prepareHeader(&hdr);
  fprintf(stderr, "seqid: %i\n", hdr.seqid);
  hdr.cmd = Command::Figure;
  if (!lau.send(&hdr)) {
    fprintf(stderr, "failed to send");
    return 1;
  }
  fprintf(stderr, "sent: %i\n", hdr.cmd);
  if (!lau.receive()) {
    fprintf(stderr, "failed to receive");
    return 1;
  }
  fprintf(stderr, "returned: %i %i %lu\n", lau.receivedHeader().seqid,
	 lau.receivedHeader().cmd,
	 lau.receivedHeader().local1);

  sleep(2);

  lau.prepareHeader(&hdr);
  fprintf(stderr, "seqid: %i\n", hdr.seqid);
  double xx[] = { 0, 1, 2, 3 };
  double yy[] = { 0, 2, 1, 3 };
  Launcher::prepareHeader(&hdr);
  hdr.cmd = Command::Line;
  hdr.nargs = 2;
  hdr.argtype[0] = hdr.argtype[1] = Launcher::DT_Double;
  hdr.arglen[0] = hdr.arglen[1] = 4*sizeof(double);
  lau.send(&hdr, xx, yy);
  fprintf(stderr, "sent: %i\n", hdr.cmd);

  if (!lau.receive()) {
    fprintf(stderr, "failed to receive");
    return 1;
  }
  fprintf(stderr, "returned: %i %i %lu\n", lau.receivedHeader().seqid,
	 lau.receivedHeader().cmd,
	 lau.receivedHeader().local1);

  fprintf(stderr, "Press return\n");
  fgetc(stdin);
  fprintf(stderr, "Deleting\n");
  return 0;
}

int silly() {
  Launcher::Header hdr;
  Launcher::prepareHeader(&hdr);
  fprintf(stderr, "seqid: %i\n", hdr.seqid);
  hdr.cmd = Command::Figure;
  fwrite(&hdr, sizeof(Launcher::Header), 1, stdout);
  fflush(stdout);
  return 0;
}

int main() {
  int res = run();
  fprintf(stderr, "Press return\n");
  fgetc(stdin);
  fprintf(stderr, "Exiting\n");
  return res;
}

