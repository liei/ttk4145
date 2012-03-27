#include <stdlib.h>
#include <stdio.h>


#include <manager.h>




global list_t *g_peers;

global peer_t *g_master;

global list_t *g_orders;


/*
 * Discover peers on the network, and find the master manager, if no
 * master manager is found, inits a new master manager.  
 * What if peers are found, but no master manager?
 *
 * starts the alive broadcasting also.
 */
manager_t *discover();
