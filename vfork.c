#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	int g_val = 100;
	pid_t id = vfork();
	if (id == 0)
	{
		g_val++;
		printf("child runing first...%d:%p\n", g_val, &g_val);
		sleep(1);
		//exit(0);
		return 0;
	}
	else
	{
		printf("father runing first...%d:%p\n", g_val, &g_val);
	}
	
	return 0;
}
