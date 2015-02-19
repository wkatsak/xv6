#include "types.h"
#include "user.h"
#include "signal.h"

// You must implement your restorer function in restorer.h
#include "restorer.h"

void handle_signal(int signum)
{
    __asm__ ("movl $0x0,%ecx\n\t");
	// Add your code to skip the return ip here
} 

int main(void)
{
    register int ecx asm ("%ecx");
    
    signal(-1, (sighandler_t) restorer);   // save the address of restorer function inside the kernel.
    signal(SIGFPE, handle_signal);         // register the actual signal for divide by zero.

    int x = 5;
    int y = 0;

    ecx = 5;
    x = x / y;

    if (ecx == 5)
        printf(1, "TEST PASSED: Final value of ecx is %d...\n", ecx);
    else
        printf(1, "TEST FAILED: Final value of ecx is %d...\n", ecx);

    exit();
}
