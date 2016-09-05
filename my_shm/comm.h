#pragma once

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define _PATH_NAME_ "/tmp"
#define _PROJ_ID_ 0x666

int create_shm(int size);
int get_shm();
int destroy_shm(int shm_id);
void * at(int shm_id);
int dt(void *addr);
