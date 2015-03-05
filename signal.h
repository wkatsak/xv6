#ifndef XV6_SIGNAL
#define XV6_SIGNAL

#define SIGKILL	0
#define SIGFPE	1

typedef void (*sighandler_t)(int);

#endif
