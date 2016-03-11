#include "types.h"
#include "stat.h"
#include "user.h"
#include "signal.h"

void dummy(void)
{
	printf(1, "TEST FAILED: this should never execute.\n");
}

void handle_signal(siginfo_t info)
{
	printf(1, "Caught signal %d...\n", info.signum);
	if (info.signum == SIGFPE)
		printf(1, "TEST PASSED\n");
	else
		printf(1, "TEST FAILED: wrong signal sent.\n");
	exit();
}

int main(int argc, char *argv[])
{
	int x = 5;
	int y = 0;

	signal(SIGFPE, handle_signal);

	x = x / y;

	printf(1, "TEST FAILED: no signal sent.\n");
	
	exit();
}
