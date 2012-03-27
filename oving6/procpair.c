#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <unistd.h>

#define MSG_SIZE 16
#define DEFAULT_KEY "/procpair"
#define N 50

int slave_wait(mqd_t mq){
  char msg[16];
  while(1){
	int rcvd = mq_receive(mq,msg,16,0);
	printf("slave: %s, rcvd %d\n",msg,rcvd);
	sleep(1);
  }
  return 0;
}

void print_numbers(mqd_t mq, int start){
  int n = N;
  char msg[MSG_SIZE + 1];
  for(int i = start; i < n; i++){
	printf("%d\n",(int)mq);
	sprintf(msg,"%d",i);
	printf("master: %s, .. ",msg);
	int sent = mq_send(mq,msg,MSG_SIZE,0);
	printf("sent: %d\n",sent);
	sleep(1);
  }
}

int main(int argc, char **argv){
  int opt = 0;
  char key[64];
  strncpy(key,DEFAULT_KEY,64);

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
	  strncpy(optarg,key,64);
	  break;
	default:
	  printf("Unknown option: %c\n",opt);
	}
  }


  //setup message queue
  mqd_t mq;
  if(master){
	
	struct mq_attr attr;
    /* initialize the queue attributes */
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MSG_SIZE;
    attr.mq_curmsgs = 0;

    /* create the message queue */
	if((mq = mq_open(DEFAULT_KEY,O_CREAT | O_WRONLY, 0644, &attr)) == (mqd_t)-1){
	  printf("Couldn't open message queue: %s\n",DEFAULT_KEY);
	  return EXIT_FAILURE;
	}
  } else {
	if((mq = mq_open(DEFAULT_KEY,O_RDWR)) == (mqd_t)-1){
	  printf("Couldn't open message queue: %s\n",DEFAULT_KEY);
	  return EXIT_FAILURE;
	}	
  }
  printf("%d\n",mq);
  int start = 0;
  if(!master){
	start = slave_wait(mq);
  }
  
  print_numbers(mq,start);

  return EXIT_SUCCESS;
}
