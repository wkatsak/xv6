#include "types.h"
#include "stat.h"
#include "user.h"
#include "signal.h"

#define NUM_EXCEPTIONS 100000

unsigned counter = 0;

void handle_signal(int signum)
{
    unsigned addr_signum, addr_retaddr, *retaddr;

    __asm__ ("movl $0xFFFF,%eax\n\t");
    __asm__ ("movl $0xFFFF,%ecx\n\t");
    __asm__ ("movl $0xFFFF,%edx\n\t");

    if (counter < NUM_EXCEPTIONS) {
        counter++;
        return;
    }

    printf(1, "Executed handler %d times, done...\n", counter);

    addr_signum = (unsigned) &signum;
    addr_retaddr = addr_signum + 16;
    retaddr = (unsigned*) addr_retaddr;
    *retaddr += 2;

    return;
} 

int main(void)
{
    register uint eax asm ("%eax");
    register uint ecx asm ("%ecx");
    register uint edx asm ("%edx");

    uint saved_ecx = 0;
    uint saved_eax = 0;
    uint saved_edx = 0;

    signal(SIGFPE, handle_signal);

    __asm__ ("movl $0x21,%eax\n\t");
    __asm__ ("movl $0x0,%ecx\n\t");
    __asm__ ("movl $0x8,%edx\n\t");
    __asm__ ("div %ecx");

    saved_ecx = ecx;
    saved_eax = eax;
    saved_edx = edx;

    printf(1, "Final register values are: eax=%x, edx=%x, ecx=%x...\n", saved_eax, saved_edx, saved_ecx);
    
    if (!counter)
        printf(1, "TEST FAILED: handler never executed\n");
    else if (saved_eax != 0x0 || saved_edx != 0x8 || saved_ecx != 0x0)
        printf(1, "TEST FAILED: registers are not as expected...\n", saved_eax, saved_edx, saved_ecx);
    else if (saved_eax == 0x0 && saved_edx == 0x8 && saved_ecx == 0x0)
        printf(1, "TEST PASSED\n");
    else
        printf(1, "TEST FAILED: weird case...\n");

    exit();
}