#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void print(sigset_t *pending)
{
	int i = 1;
	for (; i <= 31; i++)
	{
		if (sigismember(pending, i))
			printf("1");
		else
			printf("0");
	}
	printf("\n");
}

int main()
{
	sigset_t set, oset;
	sigemptyset(&set);
	sigaddset(&set, SIGINT/*2*/);

	sigprocmask(SIG_BLOCK, &set, &oset);
	
	sigset_t pending;
	sigemptyset(&pending);
	int i = 0;
	while (1)
	{
		sigpending(&pending);
		print(&pending);
		sleep(1);
		if (i++ == 10)
		{
			sigprocmask(SIG_SETMASK, &oset, NULL);
		}
	}

	return 0;
}
