#include "types.h"
#include "user.h"
#include "signal.h"

void handle_signal(int signum)
{
    unsigned addr_signum = (unsigned) &signum;
    unsigned addr_retaddr = addr_signum + 16;
    unsigned *retaddr = (unsigned*) addr_retaddr;
    printf(1, "addr_signum = %d\n", addr_signum);
    printf(1, "addr_retaddr = %d\n", addr_retaddr);
    printf(1, "retaddr = %d\n", *retaddr);
    *retaddr += 4;

    __asm__ ("movl $0x0,%ecx\n\t");
} 

int main(void)
{
    register int ecx asm ("%ecx");
    
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
