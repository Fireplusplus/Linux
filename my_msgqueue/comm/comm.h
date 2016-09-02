#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define _PATH_NAME_ "./comm/comm.h"
#define _PROJ_ID_ 0x111
#define _SIZE_ 1024

extern int ser_type;
extern int cli_type;


struct msgbuf
{
	long mtype;
	char mtext[_SIZE_];
};

int create_msg();
int get_msg();
int send_msg(int id, char *buf, int type);
int recv_msg(int id, char *buf_out, int type);
int destory_msg(int id);
