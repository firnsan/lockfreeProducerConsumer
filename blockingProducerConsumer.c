#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM 10

char queue[NUM];
sem_t blank_number, product_number;

void *producer(void *arg)
{
	int i, p=0;
	while(1) {
		sem_wait(&blank_number);
		queue[p]='1';
		write(STDOUT_FILENO, queue, NUM);
		printf("\n");
		sem_post(&product_number);
		p=(p+1)%NUM;
		sleep(1);
	}
}

void *consumer(void *arg)
{
	int c=0;
	while(1) {
		sem_wait(&product_number);
		queue[c]='0';
		//printf("consume: %d\n", queue[c]);
		sem_post(&blank_number);
		c=(c+1)%NUM;
		sleep(10);
	}
}

int main(int argc, char *argv[])
{
	memset(queue, '0', sizeof(queue));
	pthread_t pid, cid;
	sem_init(&blank_number, 0, NUM);
	sem_init(&product_number, 0, 0);

	pthread_create(&pid, NULL, producer, NULL);
	pthread_create(&cid, NULL, consumer, NULL);
	pthread_join(pid, NULL);
	pthread_join(cid, NULL);

	sem_destroy(&blank_number);
	sem_destroy(&product_number);

	return 0;
}

