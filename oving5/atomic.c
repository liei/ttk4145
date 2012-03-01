#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#ifndef THREADS
#define THREADS 3
#endif

#ifndef N
#define N 20
#endif

static int failed = 0;
static pthread_barrier_t barrier;

void start_routine(void *arg){
  int id = *(int*)(arg);
  int lastn = 0;
  int n = 0;
  printf("start t%d, [n:%d]\n",id,n);
  do{
	if(rand() % 10 == 0){
	  failed++;
	  printf("failure in t%d\n",id);
	} else {
	  n = n + 1;
	}

	//start atomic
	pthread_barrier_wait(&barrier);
	if(failed){
	  n = lastn;
	  printf("t%d, safe[n:%d]\n",id,n);
	} else {
	  printf("t%d[n:%d]\n",id,n);
	}
	pthread_barrier_wait(&barrier);
	//reset failed 
	failed = 0;
	//end atomic
	
	lastn = n;
	sleep(1);
  } while(n < N);
  pthread_exit(NULL);
}

int main(int argc, char **argv){
  srand(time(NULL));

  //init barrier
  pthread_barrier_init(&barrier,NULL,THREADS);


  pthread_t thread[THREADS];

  for(int id = 0; id < THREADS; id++){
	pthread_create(&thread[id],NULL,(void *)(&start_routine),(void *)(&id));
  }

  for(int id = 0; id < THREADS; id++){
	  pthread_join(thread[id],NULL);
  }
  return EXIT_SUCCESS;
}
