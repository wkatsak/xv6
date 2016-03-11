#include "types.h"
#include "stat.h"
#include "user.h"
#include "signal.h"

volatile int flag = 0;

void dummy(void)
{
        printf(1, "TEST FAILED: this should never execute.\n");
}

void handle_fpe(siginfo_t info)
{
	int i;

	printf(1, "Caught signal %d...\n", info.signum);

	for (i=0; i<10000000; i++) {
		continue;
	}
}

void handle_alarm(siginfo_t info)
{
	static int counter = 0;

	printf(1, "Caught signal %d...\n", info.signum);

	if (counter >= 15) {
		printf(1, "COMPLETED\n");
		exit();
	}

	counter++;
	alarm(1);
}

int main(int argc, char *argv[])
{
	int x = 5, y = 0;

	signal(SIGFPE, handle_fpe);
	signal(SIGALRM, handle_alarm);

	alarm(1);

	x = x / y;

	exit();
}
