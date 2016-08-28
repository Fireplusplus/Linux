#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	pid_t id = fork();
	if (id < 0)
	{
		perror("fork");
		return 1;
	}
	else if (id == 0) // child
	{
		printf("child exec ...\n");
		char * const argv[] = {
			"ls",
			"-l",
			NULL
		};
		char * const envp[] = {
			"/bin"
		};
		execve("/bin/ls", argv, envp);
		//execv("/bin/ls", argv);
		//execl("/bin/ls", "ls", "-l", NULL);
		printf("exec error\n");
	}
	else		  // father
	{
		printf("father wait ...\n");
		int ret = waitpid(id, NULL, 0);
		if (ret < 0)
		{
			perror("wait");
			return 2;
		}
		else
		{
			printf("wait %d success...\n", ret);
		}
	}

	return 0;
}
