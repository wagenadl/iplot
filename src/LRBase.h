// LRBase.h

#ifndef LRBASE_H

#define LRBASE_H

#include <stdint.h>

class LRBase {
public:
  enum DataType {
    DT_Char = 0,
    // DT_UChar = 1,
    // DT_Int16 = 2,
    // DT_UInt16 = 3,
    DT_Int32 = 4,
    // DT_UInt32 = 5,
    // DT_Int64 = 6,
    DT_UInt64 = 7,
    DT_Float = 8,
    DT_Double = 9,
    // DT_ComplexFloat = 10,
    // DT_ComplexDouble = 11,
  };

  static const int MaxArgs = 2;
  
  struct Header {
    uint64_t seqid; // +0
    uint64_t handle; // +8
    uint32_t arglen[MaxArgs]; // (in bytes; undefined beyond nargs)
    uint8_t argtype[MaxArgs]; // (of type DataType; undefined beyond nargs)
    uint8_t cmd; // (type command)
    uint8_t nargs; // (counts nonlocal arguments)
  };

  static void prepareHeader(Header *);
public:
  LRBase();
  bool ok() const { return ok_; }
  bool closed() const { return clsd; }
  int sendfd() const { return sfd; }
  int recvfd() const { return rfd; }
  bool send(Header const *header,
	    void const *arg1=0, void const *arg2=0, void const *arg3=0);
  bool receive(int timeout_ms=30000); // 0: don't wait
  Header const &receivedHeader() const { return rheader; }
  void const *receivedArg(int k) const;
  double retrieveDouble(int k, int n) const;
  int retrieveInt(int k, int n) const;
  int retrieveCount(int k) const;
  bool retrieveDoubles(int k, double *) const;
  void clearReceived();
protected:
  bool ok_;
  bool clsd;
  int sfd;
  int rfd;
  Header rheader;
  void *rarg[3];
};

#endif
