// Launcher.h

#ifndef LAUNCHER_H

#define LAUNCHER_H

#include "LRBase.h"
#include <unistd.h>

class Launcher: public LRBase {
public:
  Launcher(char const *command);
  ~Launcher();
  int pid() const { return pid_; }
private:
  pid_t pid_;
};

#endif
