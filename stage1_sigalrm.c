#include "types.h"
#include "stat.h"
#include "user.h"
#include "signal.h"

volatile int flag = 0;

void dummy(void)
{
	printf(1, "TEST FAILED: this should never execute.\n");
}

void handle_signal(siginfo_t info)
{
	printf(1, "Caught signal %d...\n", info.signum);
	if (info.signum == SIGALRM)
		printf(1, "TEST PASSED\n");
	else
		printf(1, "TEST FAILED: wrong signal sent.\n");
	exit();
}

int main(int argc, char *argv[])
{
	int start = uptime();

	signal(SIGALRM, handle_signal);

	alarm(1);

	while(!flag && uptime() < start + 2000);

	printf(1, "TEST FAILED: no signal sent.\n");
	
	exit();
}
