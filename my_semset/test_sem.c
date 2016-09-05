#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "comm.h"
 
int main()
{
	int sem_id = create_sem_set(1);
	init_sem_set(sem_id, 0, 1);

	pid_t id = fork();
	if (id < 0)
	{
		perror("fork");
		return 1;
	}
	else if (id == 0) // child
	{
		while (1)
		{
			P(sem_id);

			printf("A");
			fflush(stdout);
			usleep(rand()%12345);
			usleep(200000);
			printf("A");
			fflush(stdout);
			usleep(rand()%12345);

			V(sem_id);
		}
	}
	else // father
	{
		while (1)
		{
			P(sem_id);

			printf("B");
			fflush(stdout);
			usleep(rand()%12345);
			usleep(200000);
			printf("B");
			fflush(stdout);
			usleep(rand()%12345);

			V(sem_id);
		}
	}

	
	destroy(sem_id);
	return 0;
}
