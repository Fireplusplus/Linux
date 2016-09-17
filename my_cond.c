#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct _list
{
	struct _list *next;
	int _val;
}product_list;

product_list *head = NULL;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t need_product=PTHREAD_COND_INITIALIZER;

void init_list(product_list *list)
{
	if (NULL != list)
	{
		list->next = NULL;
		list->_val = 0;
	}
}

void *consumer(void *_val)
{
	product_list *p = NULL;
	while (1)
	{
		pthread_mutex_lock(&lock);
		while (NULL == head)
		{
			pthread_cond_wait(&need_product, &lock);
		}
		p = head;
		head = head->next;
		p->next = NULL;
		printf("comsum success, val is : %d\n", p->_val);
		pthread_mutex_unlock(&lock);
		free(p);
		p = NULL;
	}

	return NULL;
}

void *product(void *_val)
{
	while (1)
	{
		sleep(rand() % 2);
		product_list *p = malloc(sizeof(product_list));
		pthread_mutex_lock(&lock);
		init_list(p);
		p->_val = rand() % 1000;
		p->next = head;
		head = p;
		printf("call consumer! product success, val is : %d\n",
					p->_val);
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&need_product);
	}
}

int main()
{
	pthread_t t_product;
	pthread_t t_consumer;
	pthread_create(&t_product, NULL, product, NULL);
	pthread_create(&t_consumer, NULL, consumer, NULL);

	pthread_join(t_product, NULL);
	pthread_join(t_consumer, NULL);

	return 0;
}




