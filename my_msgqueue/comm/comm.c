#include "comm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cli_type = 1;
int ser_type = 2;


static int comm_msg(int flag)
{
	int key = ftok(_PATH_NAME_, _PROJ_ID_);	
	if (key < 0)
	{
		perror("ftok");
		exit(1);
	}
	
	int id = msgget(key, flag);
	if (id < 0)
	{
		perror("msgget");
		exit(2);
	}
	
	return id;
}

int create_msg()
{
	int flag = IPC_CREAT | IPC_EXCL | 0666; 
	return comm_msg(flag);
}

int get_msg()
{
	int flag = IPC_CREAT; 
	return comm_msg(flag);
}

int send_msg(int id, char *buf, int type)
{
	struct msgbuf msg;
	msg.mtype = type;
	strncpy(msg.mtext, buf, strlen(buf)+1);

	int ret = msgsnd(id, &msg, sizeof(msg.mtext), 0);	// 0缺省阻塞
	if (ret < 0)
	{
		perror("msgsnd");
		return ret;
	}
	return 0;
}


int recv_msg(int id, char *buf_out, int type)
{
	struct msgbuf msg;
	size_t _s = msgrcv(id, &msg, sizeof(msg.mtext), type, 0);
	if (_s < 0)
	{
		perror("msgrcv");
		return _s;
	}
	strcpy(buf_out, msg.mtext);
}	

int destory_msg(int id)
{
	msgctl(id, IPC_RMID, NULL);
}
