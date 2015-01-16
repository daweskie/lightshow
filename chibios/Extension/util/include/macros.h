#ifndef MACROS_H_INCLUDED
#define MACROS_H_INCLUDED

#define MALLOC(ptr,size) \
  do { \
     ptr=chCoreAlloc((size));\
  } while(0)

#define min(a,b) (a)<(b) ? (a) : (b)

#define max(a,b) (a)<(b) ? (b) : (a)

#ifdef DEBUG
#define dbg(msg,...) \
chprintf(dbgdisp,msg,##__VA_ARGS__)
#else
#define dbg(msg,...)
#endif

#endif // MACROS_H_INCLUDED
