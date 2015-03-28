// LRBase.cpp

#include "LRBase.h"
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/select.h>
#include <stdlib.h>
#include <string.h>

LRBase::LRBase() {
  ok_ = false;
  clsd = false;
  for (int k=0; k<3; k++)
    rarg[k] = 0;
}

bool LRBase::send(Header const *header,
		  void const *arg1, void const *arg2, void const *arg3) {
  if (clsd || !ok_)
    return false;

  if (header->nargs > MaxArgs) {
    fprintf(stderr,"LRBase: argument count problem\n");
    return false;
  }
  
  ssize_t n = write(sfd, (void const*)header, sizeof(Header));
  if (n<0) {
    perror("LRBase");
    ok_ = false;
    return false;
  } else if (n==0) {
    fprintf(stderr, "0 on writing??\n");
    ok_ = false;
    clsd = true;
    return false;
  } else if (n<(ssize_t)sizeof(Header)) {
    fprintf(stderr,
	    "LRBase: Unexpected partial write. Disaster imminent.\n");
    ok_ = false;
    return false;
  }
  // got header

  void const *ptrs[3] = { arg1, arg2, arg3 };
  for (int k=0; k<header->nargs; k++) {
    char const *ptr = (char const *)ptrs[k];
    int N = header->arglen[k];
    while (N>0) {
      ssize_t n = write(sfd, ptr, N);
      if (n<0) {
	perror("LRBase");
	ok_ = false;
	return false;
      } else if (n==0) {
	clsd = true;
	return false;
      }
      ptr += n;
      N -= n;
    }
  }
  return true;
}

bool LRBase::receive(int timeout_ms) {
  if (clsd || !ok_)
    return false;
  
  clearReceived();

  fd_set readfds;
  FD_ZERO(&readfds);
  FD_SET(rfd, &readfds);
  struct timeval to;
  to.tv_sec = timeout_ms/1000;
  to.tv_usec = (timeout_ms%1000)*1000;
  int r = select(rfd+1, &readfds, 0, 0, &to);
  if (r<0)
    perror("LRBase (select)");
  if (r<=0)
    return false;

  // so we have something to read
  ssize_t n = read(rfd, &rheader, sizeof(Header));
  if (n<ssize_t(sizeof(Header))) {
    if (n<0) {
      perror("LRBase");
      ok_ = false;
    } else if (n>0) {
      fprintf(stderr,
	      "LRBase: Unexpected partial read. Disaster imminent.\n");
      ok_ = false;
    } else { // EOF
    clsd = true;
    }
    return false;
  }

  for (int k=0; k<rheader.nargs; k++) {
    ssize_t N = rheader.arglen[k];
    rarg[k] = malloc(N);
    if (rarg[k]==0) {
      fprintf(stderr, "LRBase: Memory allocation failure for %li bytes.\n", N);
      ok_ = false;
      return false;
    }
    char *ptr = (char*)rarg[k];
    while (N>0) {
      ssize_t n = read(rfd, ptr, N);
      if (n<0) {
	perror("LRBase");
	ok_ = false;
	return false;
      } else if (n==0) {
	fprintf(stderr, "LRBase: Unexpected EOF.\n");
	ok_ = false;
	clsd = true;
	return false;
      }
      ptr += n;
      N -= n;
    }
  }
  return true;
}

void LRBase::clearReceived() {
  for (int n=0; n<3; n++)
    free(rarg[n]);
  for (int n=0; n<3; n++)
    rarg[n] = 0;  
}

int LRBase::retrieveCount(int k) const {
  if (k>=rheader.nargs)
    return 0;

  unsigned int L = rheader.arglen[k];

  switch (DataType(rheader.argtype[k])) {
  case DT_Char: return L/sizeof(char);
  case DT_Int32: return L/sizeof(int32_t);
  case DT_UInt64: return L/sizeof(uint64_t);
  case DT_Float: return L/sizeof(float);
  case DT_Double: return L/sizeof(double);
  }
  return 0; // not executed
}


double LRBase::retrieveDouble(int k, int n) const {
  if (k>=rheader.nargs)
    return nan("");
  unsigned int L = rheader.arglen[k];
  switch (DataType(rheader.argtype[k])) {
  case DT_Char:
    if (n*sizeof(char)<L)
      return double( ((char const *)rarg[k])[n] );
    break;
  case DT_Int32:
    if (n*sizeof(int32_t)<L)
      return double( ((int32_t const *)rarg[k])[n] );
    break;
  case DT_Float:
    if (n*sizeof(float)<L)
      return double( ((float const *)rarg[k])[n] );
    break;
  case DT_Double:
    if (n*sizeof(double)<L)
      return double( ((double const *)rarg[k])[n] );
    break;
  case DT_UInt64:
    if (n*sizeof(uint64_t)<L)
      return double( ((uint64_t const *)rarg[k])[n] );
    break;
  }
  return nan("");
}

int LRBase::retrieveInt(int k, int n) const {
  if (k>=rheader.nargs)
    return 0;
  unsigned int L = rheader.arglen[k];
  switch (DataType(rheader.argtype[k])) {
  case DT_Char:
    if (n*sizeof(char)<L)
      return int( ((char const *)rarg[k])[n] );
    break;
  case DT_Int32:
    if (n*sizeof(int)<L)
      return int( ((int32_t const *)rarg[k])[n] );
    break;
  case DT_Float:
    if (n*sizeof(float)<L)
      return int( ((float const *)rarg[k])[n] );
    break;
  case DT_Double:
    if (n*sizeof(int)<L)
      return int( ((double const *)rarg[k])[n] );
    break;
  case DT_UInt64:
    if (n*sizeof(uint64_t)<L)
      return int( ((uint64_t const *)rarg[k])[n] );
    break;
  }
  return 0;
}

void LRBase::prepareHeader(LRBase::Header *hdr) {
  static uint32_t seqid = 1;

  seqid ++;
  hdr->seqid = seqid;
  hdr->cmd = 0;
  hdr->nargs = 0;
  hdr->handle = 0;
}

void const *LRBase::receivedArg(int k) const {
  return (k>=0 && k<MaxArgs) ? rarg[k] : 0;
}

bool LRBase::retrieveDoubles(int k, double *dst) const {
  void const *s = receivedArg(k);
  if (!s)
    return false;
  
  int N = retrieveCount(k);
  switch ((DataType)rheader.argtype[k]) {
  case DT_Char: {
    char const *src = (char const *)s;
    for (int n=0; n<N; n++)
      *dst++ = *src++;
  } break;
  case DT_Int32: {
    int32_t const *src = (int32_t const *)s;
    for (int n=0; n<N; n++)
      *dst++ = *src++;
  } break;
  case DT_UInt64: {
    uint64_t const *src = (uint64_t const *)s;
    for (int n=0; n<N; n++)
      *dst++ = *src++;
  } break;
  case DT_Float: {
    float const *src = (float const *)s;
    for (int n=0; n<N; n++)
      *dst++ = *src++;
  } break;
  case DT_Double:
    memcpy(dst, s, N*sizeof(double));
    break;
  default:
    return false;
  }
  return true;
}

//double *LRBase::easyDoubles(int k) {
//  void *res = receivedArgument(k);
//  if (res && argtype[k]==DT_Double)
//    return res;
//  else
//    return NULL;
//}

    
  
