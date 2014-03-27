#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define NUM 10
#define CAS(addr, old, new) __sync_bool_compare_and_swap((addr), (old), (new))

char queue[NUM];
int blank_number, product_number;

void *producer(void *arg)
{
	int p=0;
	int old_blank_number, new_blank_number;
	int old_product_number, new_product_number;
	while(1) {
		do {
			while(!blank_number) ; //while full
			old_blank_number=blank_number;
			new_blank_number=old_blank_number-1;
		} while(!CAS(&blank_number, old_blank_number, new_blank_number));
		
		queue[p]='1';
		write(STDOUT_FILENO, queue, NUM);
		printf("\n");
		do {
			old_product_number=product_number;
			new_product_number=old_product_number+1;
		} while(!CAS(&product_number, old_product_number, new_product_number));
		p=(p+1)%NUM;
		sleep(rand()%5);
	}
}

void *consumer(void *arg)
{
	int c=0;
	int old_blank_number, new_blank_number;
	int old_product_number, new_product_number;
	while(1) {
		do {
			while(!product_number);
			old_product_number=product_number;
			new_product_number=old_product_number-1;
		} while(!CAS(&product_number, old_product_number, new_product_number));
		queue[c]='0';
		
		do {
			old_blank_number=blank_number;
			new_blank_number=old_blank_number+1;
		} while(!CAS(&blank_number, old_blank_number, new_blank_number));
		c=(c+1)%NUM;
		sleep(rand()%5);
	}
}

int main(int argc, char *argv[])
{
	memset(queue, '0', sizeof(queue));
	pthread_t pid, cid;
	product_number=0;
	blank_number=NUM;

	pthread_create(&pid, NULL, producer, NULL);
	pthread_create(&cid, NULL, consumer, NULL);
	pthread_join(pid, NULL);
	pthread_join(cid, NULL);


	return 0;
}

