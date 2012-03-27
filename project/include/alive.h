#ifndef _ALIVE_H_
#define _ALIVE_H_


#define ALIVE_ADDRESS "129.241.187.255"
#define ALIVE_MESSAGE "alive"
#define ALIVE_TIMEOUT 250 // ms

typedef struct {
  int id;
  char msg[sizeof(ALIVE_MESSAGE)];
} alive_msg_t;

pthread_t alive_broadcaster;
pthread_t alive_listener;


/*
 * Start broadcasting alive messages over udp.
 */
void startAliveBroadcaster(int id);

/*
 * Start listening for broadcasted alive messages.
 */
void startAliveListener();





#endif // _ALIVE_H_
