#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <mqueue.h>
#include <unistd.h>
#include <getopt.h>

#define QUEUE_NAME  "/test_queue"
#define MAX_SIZE    32
#define MSG_STOP    "exit"

#define CHECK(x) \
    do { \
        if (!(x)) { \
            fprintf(stderr, "%s:%d: ", __func__, __LINE__); \
            perror(#x); \
            exit(-1); \
        } \
    } while (0) \

void create_mq(mqd_t *mq,char *name){

  struct mq_attr attr; 
  /* initialize the queue attributes */
  attr.mq_flags = 0;
  attr.mq_maxmsg = 10;
  attr.mq_msgsize = MAX_SIZE;
  attr.mq_curmsgs = 0;

  /* create the message queue */
  *mq = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0644, &attr);
  CHECK((mqd_t)-1 != *mq);
}

void print_numbers(mqd_t mq, int start){
  char buffer[MAX_SIZE];
 
  int n = 10;
  for(int i = start; i < n; i++){
	memset(buffer, 0, MAX_SIZE);
	sprintf(buffer,"%d",i);
	printf("msg: %s\n",buffer);
	/* send the message */
	CHECK(0 <= mq_send(mq, buffer, MAX_SIZE, 0));
	sleep(1);
  } 

  CHECK(0 <= mq_send(mq,MSG_STOP,MAX_SIZE,0));
  /* cleanup */
  CHECK((mqd_t)-1 != mq_close(mq));
  CHECK((mqd_t)-1 != mq_unlink(QUEUE_NAME));
}


void connect_mq(mqd_t *mq,char *name){
  /* open the mail queue */
  *mq = mq_open(name, O_RDWR | O_NONBLOCK);
  CHECK((mqd_t)-1 != *mq);
}

int slave_wait(mqd_t mq){
  char buffer[MAX_SIZE + 1];
  int lastNumber;
  int failedReads = 0;
  while(failedReads < 6){
	ssize_t bytes_read;

	/* receive the message */
	bytes_read = mq_receive(mq, buffer, MAX_SIZE, NULL);
	if(bytes_read == -1){
	  failedReads++;
	  if(failedReads == 2){
		printf("waiting");
	  } else if (failedReads > 2){
		printf(".");
	  }
	  fflush(stdout);
	  sleep(1);
	  continue;
	}
	failedReads = 0;
	buffer[bytes_read] = '\0';
	if(!strncmp(buffer,MSG_STOP,strlen(MSG_STOP))){
	  printf("Master exited successfully\n");
	  CHECK((mqd_t)-1 != mq_close(mq));
	  exit(EXIT_SUCCESS);
	}
	lastNumber = atoi(buffer);
  }
  printf("\n");
  return lastNumber + 1;
}

int main(int argc, char **argv) {
  int opt = 0;

  int master = 0;

  while(opt != -1){
	opt = getopt(argc,argv,"msk:");
	switch(opt){
	case -1:
	  break;
	case 'm':
	  master = 1;
	  break;
	case 's':
	  master = 0;
	  break;
	case 'k':
	  printf("key [%s]\n",optarg);
	  break;
	default:
	  printf("Unknown option: %c\n",opt);
	}
  }

  mqd_t mq; 

  if(master){
	create_mq(&mq,QUEUE_NAME);
  }	else {
	connect_mq(&mq,QUEUE_NAME);
  }
  int start = 0;
  if(!master){
	start = slave_wait(mq);
  }
  
  print_numbers(mq,start);
}
