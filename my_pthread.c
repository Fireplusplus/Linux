#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int g_val = 0;
pthread_mutex_t lock;

void* pthread_run(void* arg)
{
	// pthread_detach(pthread_self());
	printf("i am new pthread\n");
	int i = 0;
	while (i++ < 5000)
	{
		pthread_mutex_lock(&lock);

		int tmp = g_val;
		printf("id: %lu, g_val = %d\n", pthread_self(), g_val);
		g_val = tmp + 1;

		pthread_mutex_unlock(&lock);
	}

	pthread_exit((void*)1);
}

int main()
{
	pthread_mutex_init(&lock, NULL);

	pthread_t id1, id2;
	pthread_create(&id1, NULL, pthread_run, NULL);
	pthread_create(&id2, NULL, pthread_run, NULL);

	pthread_join(id1, NULL);
	pthread_join(id2, NULL);
	printf("g_val = %d\n", g_val);
	
	pthread_mutex_destroy(&lock);

	return 0;
}
