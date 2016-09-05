#include "comm.h"
#include <stdio.h>

static int comm_shm(int size, int flag)
{
	key_t key = ftok(_PATH_NAME_, _PROJ_ID_);
	if (key < 0)
	{
		perror("ftok");
		return -2;
	}

	return shmget(key, size, flag);
}

int create_shm(int size)
{
	int flag = IPC_CREAT | IPC_EXCL | 0644;
	return comm_shm(size, flag);
}


int get_shm()
{
	int flag = IPC_CREAT;
	return comm_shm(0, flag);	
}

int destroy_shm(int shm_id)
{
	return shmctl(shm_id, IPC_RMID, NULL);
}

void * at(int shm_id)
{
	return shmat(shm_id, NULL, 0);
}

int dt(void *addr)
{
	return shmdt(addr); 
}
