#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define CAP 128
int ring[128];

sem_t sem_space;
sem_t sem_data;

pthread_mutex_t pro_lock;
pthread_mutex_t con_lock;

void* product(void* arg)
{
	int index = 0;
	while (1)
	{
		sem_wait(&sem_space);

		int val = rand() % 100;

		pthread_mutex_lock(&pro_lock);
		ring[index++] = val;
		pthread_mutex_unlock(&pro_lock);

		index %= CAP;
		printf("product done ... , %d, %lu\n", val, pthread_self());

		sem_post(&sem_data);

		sleep(1);
	}
}

void* consume(void* arg)
{
	int index = 0;
	while (1)
	{
		sem_wait(&sem_data);

		pthread_mutex_lock(&con_lock);
		int val = ring[index++];
		pthread_mutex_unlock(&con_lock);

		index %= CAP;
		printf("consume done...%d, %lu\n", val, pthread_self());

		sem_post(&sem_space);

		sleep(1);
	}
}

int main()
{
	pthread_t pro, pro2;
	pthread_t con, con2;

	sem_init(&sem_space, 0, CAP);
	sem_init(&sem_data, 0, 0);
	
	pthread_mutex_init(&pro_lock, NULL);
	pthread_mutex_init(&con_lock, NULL);

	pthread_create(&pro, NULL, product, NULL);
	pthread_create(&con, NULL, consume, NULL);
	pthread_create(&pro2, NULL, product, NULL);
	pthread_create(&con2, NULL, consume, NULL);

	pthread_join(pro, NULL);
	pthread_join(con, NULL);
	pthread_join(pro2, NULL);
	pthread_join(con2, NULL);

	return 0;
}
