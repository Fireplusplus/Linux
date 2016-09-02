#include <stdio.h>
#include "comm/comm.h"
#include <unistd.h>
#include <string.h>

int main()
{
	int id = get_msg();
	printf("%d\n", id);

	char buf[_SIZE_];
	while (1)
	{
		memset(buf, 0, sizeof(buf));
		printf("Please Enter# ");
		fflush(stdout);
		ssize_t _s = read(0, buf, sizeof(buf));
		if (_s > 0)
		{
			buf[_s - 1] = 0;
		}
		send_msg(id, buf, cli_type);
		if (strcasecmp(buf, "quit") == 0)
			break;

		memset(buf, 0, sizeof(buf));
		recv_msg(id, buf, ser_type);
		printf("server# %s\n", buf);

	}

	return 0;
}
