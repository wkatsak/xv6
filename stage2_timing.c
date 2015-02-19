#include "types.h"
#include "stat.h"
#include "user.h"
#include "signal.h"

void handle_signal(int signum)
{

}

int main(int argc, char *argv[])
{
	signal(SIGFPE, handle_signal);
	
	printf(1, "Traps Performed: XXXX\n");
	printf(1, "Total Elapsed Time: XXXX\n");
	printf(1, "Average Time Per Trap: XXXXX\n");

	exit();
}