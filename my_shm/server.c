#include <stdio.h>
#include <unistd.h>
#include "comm.h"


int main()
{
	int shm_id = create_shm(4096);
	char *addr = (char *)at(shm_id);

	int i = 0;
	while (1)
	{
		addr[i] = 'A';
		addr[i + 1] = 0;
		++i;
		i %= 4096;
		sleep(1);
	}

	dt(addr);
	destroy_shm(shm_id);

	return 0;
}
