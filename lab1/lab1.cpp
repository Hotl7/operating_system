#include <iostream>
#include <pthread.h>

using namespace std;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int ready = 0;

void* producer(void* arg){
	while(1){
		pthread_mutex_lock(&lock);
		if (ready == 1) {
			pthread_mutex_unlock(&lock);
			continue;
		}
		ready = 1;
		cout<<"Поставщик отправляет событие"<<endl;
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&lock);
	}
return nullptr;
}

void* consumer(void* arg){
	while(1){
		pthread_mutex_lock(&lock);
		while(ready == 0) {
			pthread_cond_wait(&cond,&lock);
		}
		ready = 0;
		cout<<" Потребитель получил событие"<<endl;
		pthread_mutex_unlock(&lock);
	}
return nullptr;
}

int main() {
	pthread_t producerThread,consumerThread;
	
	pthread_create(&producerThread,nullptr,producer,nullptr);
	pthread_create(&consumerThread,nullptr,consumer,nullptr);
	
	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&cond);

	pthread_join(consumerThread,NULL);
	return 0;
}

