





typedef struct {
  int owner_id;
  int floor;
} order_t;



order_t *new_order(int owner_id, int floor);


void sendOrder(peer_t *peer, order_t order);

void receiveOrder(order_t *order);
