#ifndef MACROS_H_INCLUDED
#define MACROS_H_INCLUDED

#define MALLOC(ptr,size) \
  do { \
     ptr=chCoreAlloc((size));\
  } while(0)

#define min(a,b) (a)<(b) ? (a) : (b)

#define max(a,b) (a)<(b) ? (b) : (a)


#endif // MACROS_H_INCLUDED
