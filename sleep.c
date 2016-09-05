#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void sig_sleep()
{}

int my_sleep(int timeout)
{
	struct sigaction act, oact;
	act.sa_handler = sig_sleep;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGALRM, &act, &oact);
	alarm(timeout);

	pause();

	int left = alarm(0); // 清空闹钟
	sigaction(SIGALRM, &oact, NULL);
	
	return left;
}

int main()
{

	while (1)
	{
		printf("I am go to sleep...\n");
		my_sleep(1);
	}

	return 0;
}
