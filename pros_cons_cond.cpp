#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <queue>

using namespace std;

queue<int> q;

static pthread_mutex_t con_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t pro_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t need_product=PTHREAD_COND_INITIALIZER;

void *consumer(void *_val)
{
	while (1)
	{
		pthread_mutex_lock(&lock);

		while (q.empty())
		{
			pthread_cond_wait(&need_product, &lock);
		}
		pthread_mutex_lock(&con_lock);
		int tmp = q.front();
		q.pop();
		pthread_mutex_unlock(&con_lock);
		printf("comsum success, val is : %d\n", tmp);

		pthread_mutex_unlock(&lock);
		sleep(8);
	}

	return NULL;
}

void *product(void *_val)
{
	while (1)
	{
		sleep(rand()%2);
		pthread_mutex_lock(&lock);

		int tmp = rand() % 1000;
		pthread_mutex_lock(&pro_lock);
		q.push(tmp);
		pthread_mutex_unlock(&pro_lock);
		printf("call consumer! product success, val is : %d\n", tmp);

		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&need_product);
		sleep(1);
	}
}

int main()
{
	pthread_t t_product, t_product2;
	pthread_t t_consumer, t_consumer2;
	pthread_create(&t_product, NULL, product, NULL);
	pthread_create(&t_product2, NULL, product, NULL);
	pthread_create(&t_consumer, NULL, consumer, NULL);
	pthread_create(&t_consumer2, NULL, consumer, NULL);

	pthread_join(t_product, NULL);
	pthread_join(t_consumer, NULL);
	pthread_join(t_product2, NULL);
	pthread_join(t_consumer2, NULL);

	return 0;
}




