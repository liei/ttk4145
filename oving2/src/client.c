#include <stdlib.h>
#include <stdio.h>
#include <sys/unistd.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

#include <server.h>
#include <util.h>

/*
 * Summary:			This thread function implements the client application
 * 					The client tries to establish a connection to the server, and once when it gets
 * 					the client list sends messages to all clients in the list
 * Parameters:   	Client Description - ClientID / Username
 * Return:			NULL always
 */
void* ClientThreadFunc(void * pargs) {
	// Connection socket
  SOCKET ConnectSocket = INVALID_SOCKET;

  // State machine
  int		nState = cli_state_init;
  // Counter, iterator
  int		i;

  // Address info results
  struct addrinfo *result = NULL;

  // Address info hints
  struct addrinfo hints;

  // Receive Buffer
  BYTE	bRxBuffer[1024];

  // Text Json
  char * out;
  // Result from send
  int by_recv;
  // Client List structure
  jsonCLI_LIST cli_list;
  // Chat message structure
  jsonCHAT_MSG  chat_mess;
  // Timeout variables
  time_t 	tLastPingSent, tLastDataReceived;
  
  // Variable measuring loop iterations
  // to implement periodic sending of messages to other client applications
  int	mess_cnt = 1;
  
  // Buffer data pointer, the same meaning as within the SERVER Client handler Thread
  unsigned long ulNewPos = 0;
  
  PCLIENT_DESCRIPTOR	pCli = (PCLIENT_DESCRIPTOR) pargs;
  
  // Purge Buffer
  memset(bRxBuffer, 0, sizeof(bRxBuffer));
  
  // Reset Timeouts
  tLastDataReceived = time(NULL);
  tLastPingSent = 0;
  
  
  while(1) {
    // Go - Initial state is "cli_state_init"!!
    switch(nState) {
    case cli_state_init:
      // Get address info
      ZeroMemory( &hints, sizeof(hints) );
      hints.ai_family = AF_UNSPEC;
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_protocol = IPPROTO_TCP;
      
      if(getaddrinfo("localhost", "9009", &hints, &result) == SOCKET_ERROR){
	// If error - exit!
	return NULL;
      } else {
	// Else go to new state!
	nState = cli_state_create_sck;
      }
      break;
	  
    case cli_state_create_sck:
      // Create socket
      ConnectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
      
      if(ConnectSocket == INVALID_SOCKET) {
	// If error, go to initial state
	freeaddrinfo(result);
	nState = cli_state_init;
      } else {
	// If OK, proceed to the next state
	nState = cli_state_connect;
      }
      break;
    case cli_state_connect:
	  
      // Connect to Server!
      if(connect(ConnectSocket, result->ai_addr, result->ai_addrlen) == SOCKET_ERROR)
	{
	  // If connection failed - Try again!
	  nState = cli_state_connect;
	}
      else
	{
	  // Connection is established - Set the socket to NONBLOCKING!
	  nState = cli_state_connected;
	  setNonblocking(ConnectSocket);
	  // Reset Timeouts
	  tLastDataReceived = time(NULL);
	  tLastPingSent = 0;
	}
	  
      break;
      
    case cli_state_connected:
      
      // Connected - Register yourself!
      out = prepare_cli_reg_mess(pCli->strCliName);
      
      if(send(ConnectSocket, out, strlen(out), 0) == SOCKET_ERROR) {
	// If error - Clean up!
	nState = cli_state_cleanup;
      } else {
	// Go to next state
	nState = cli_state_chat;
	// Purge buffer and reset the pointer
	memset(bRxBuffer, 0, sizeof(bRxBuffer));
	ulNewPos = 0;
      }
      break;
    
    case cli_state_chat:
      // Receive data!
      by_recv = recv(ConnectSocket, &bRxBuffer[ulNewPos], sizeof(bRxBuffer)-ulNewPos, 0);
      
      if((by_recv == SOCKET_ERROR) && (errno != EAGAIN) && (errno != EWOULDBLOCK)) {
	// If error!
	nState = cli_state_cleanup;
	break;
      } else if(by_recv > 0){
	// Something received!
	PCHAR pMessage;
	// Reset timeouts!
	tLastDataReceived = time(NULL);
	tLastPingSent = 0;
	// Parse messages!
	while((pMessage = ExtractJSONFromBuffer((char*)bRxBuffer, by_recv + ulNewPos, &ulNewPos))) {
	  
	  by_recv = 0;
		  
	  if(get_message_type(pMessage) == jsonCLI_LIST_MESSAGE) {
	    // Got client list!
	    cli_list = parse_cli_list_mess(pMessage);
	    
	    printf("Client %s received list of connected clients->\n", pCli->strCliName);
	    for(i = 0; i < cli_list.count; i++) {
	      printf("\t%s\n", cli_list.list[i]);
	    }
	  }
	  else if(get_message_type(pMessage) == jsonCHAT_MESSAGE) {
	    // Got chat message!
	    chat_mess = parse_chat_message(pMessage);
	    
	    printf("RX -> To: %s; From: %s; Mess: %s\n", pCli->strCliName, chat_mess.from, chat_mess.message);
	  } else if(get_message_type(pMessage) == jsonPING_MESSAGE){
	    // Got Ping - Respond with PONG!
	    out = prepare_pong_mesage("N/a", "N/a");
	    send(ConnectSocket, out, strlen(out), 0);
	  } else if(get_message_type(pMessage) == jsonPONG_MESSAGE){
	    // Got PONG!
	    printf("CLIENT -> RECEIVED PONG...\n");
	  } else {
	    // Unknown data - Print warning!
	    printf("%s: Unknown message received\n", pCli->strCliName);
	  }
		  
	  free(pMessage);
	}
	      
	      
      } else {
	// Nothing in the socket buffer - Check timeout!
	if(((time(NULL) - tLastDataReceived) > 10) && !tLastPingSent) {
	  // Send ping if we have more than 10 seconds and no PING Sent within these 10 seconds!
	  out = prepare_ping_mesage("N/a", "N/a");
	  
	  send(ConnectSocket, out, strlen(out), 0);
	  
	  tLastPingSent = time(NULL);
	} else {
	  // Ping sent
	  if(tLastPingSent && ((time(NULL) - tLastPingSent) > 2)) {
	    // Ping sent, but nothing in response for at least 2 seconds!
	    // Close Socket and reinitialize!
	    shutdown(ConnectSocket, SHUT_RDWR);
	    closesocket(ConnectSocket);
	    
	    
	    nState = cli_state_cleanup;
	    
	    printf("No data after ping...\n");
	  }
	}
      }
      
      // Every 4 seconds generate message for each client from the list!
      // 200 * 20 msec = 4000 msec = 4 sec
      
      if(mess_cnt++ > 200){
	mess_cnt = 0;
	for(i = 0; i < cli_list.count; i++){
	  if(!strcmp(cli_list.list[i], pCli->strCliName)){
	    // Don't sent to yourself!
	    continue;
	  } else {
	    char tmp[512];
	    int  nSent;
	    
	    sprintf(tmp, "Message to %s from %s\n", cli_list.list[i], pCli->strCliName);
	    
	    printf("TX -> From: %s; To: %s; Mess: %s\n", pCli->strCliName, cli_list.list[i], tmp);
		      
	    out = prepare_chat_message(cli_list.list[i], pCli->strCliName, tmp);
		      
	    nSent = send(ConnectSocket, out, strlen(out), 0);
		      
	    if(nSent == SOCKET_ERROR){
	      printf("CLIENT -> Send() Error\n");
	      nState = cli_state_cleanup;
	    } else if(nSent != strlen(out)){
	      printf("CLIENT -> Warning (Sent: %d, ToSend: %d)\n", nSent, (int)strlen(out));
	    } else if(nSent == 0) {
	      printf("CLIENT -> Warning\n");
	    }
	  }
	}
      }
      break;

    case cli_state_cleanup:
      // Cleanup!
      // Close!
      shutdown(ConnectSocket, SHUT_RDWR);
      closesocket(ConnectSocket);
      freeaddrinfo(result);
      
      nState = cli_state_init;
      break;

    default:
      printf("Client -> Unallowed state!\n");
      break;
    }
      
    // Rest a while not to overload the CPU!
    SleepEx(20, FALSE);
  }
  
  return NULL;
}

int main(int argc, char **argv){

  pthread_t client;
  CLIENT_DESCRIPTOR desc;

  signal(SIGPIPE, SIG_IGN);



  // Start Client with the ClientID/Username fro the command line!
  strcpy(desc.strCliName, argv[1]);
  pthread_create(&client, NULL, ClientThreadFunc, &desc);
  
  
  while(1) {
    // Loop forever!
    SleepEx(1000, FALSE);
  }

}
