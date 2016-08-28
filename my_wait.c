#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
	pid_t id = fork();
	if (id < 0)
	{
		perror("fork");
		return 1;
	}
	else if (id == 0)	// child
	{
		printf("child sleep: %d\n", getpid());
		sleep(100);
		exit(0);
	}
	else			// father
	{
		printf("father wait...\n");
		int status;
		int ret = waitpid(id, &status, 0);
		if (ret < 0)
		{
			perror("waitpid");
			return 2;
		}
		if (WIFEXITED(status))
		{
			printf("child exit normal: exit code is %d\n",
				WEXITSTATUS(status));
		}
		else
		{
			if (WIFSIGNALED(status))
			{
				printf("child exit not normal: sig is %d\n",
					WTERMSIG(status));
			}
		}
	}

	return 0;
}
