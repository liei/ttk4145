







#include <recs.h>
#include <manager.h>




int main(int argc, char **argv){
  peer_t *master = discover();


  initManager(master);


  // start elevator in current main thread;
  startElevator();
}
