#include <stdio.h>
#include <unistd.h>
#include "comm.h"


int main()
{
	int shm_id = get_shm();
	char *addr = (char *)at(shm_id);

	int i = 0;
	while (1)
	{
		printf("%s\n", addr);
		sleep(1);
	}

	dt(addr);

	return 0;
}
