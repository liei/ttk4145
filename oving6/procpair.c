#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <unistd.h>


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
  printf("enter print_numbers");
  int n = N;
  char msg[16];
  for(int i = start; i < n; i++){
	sprintf(msg,"%d",i);
	printf("master: %s, .. ",msg);
	int sent = mq_send(mq,msg,16,0);
	printf("sent: %d\n",sent);
	sleep(2);
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
	if((mq = mq_open(key,(O_RDWR | O_CREAT),0777,NULL)) == -1){
	  printf("Couldn't open message queue: %s\n",key);
	  return EXIT_FAILURE;
	}
	printf("opened mq\n");
  } else {
	if((mq = mq_open(key,O_RDWR)) == -1){
	  printf("Couldn't open message queue: %s\n",key);
	  return EXIT_FAILURE;
	}	
  }
 
  printf("what\n");
  int start = 0;
  if(!master){
	printf("wtf\n");
	start = slave_wait(mq);
  }
  printf("before pn\n");
  print_numbers(mq,start);

  return EXIT_SUCCESS;
}
