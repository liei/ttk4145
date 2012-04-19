package edu.ntnu.ttk4145.recs.message;

import edu.ntnu.ttk4145.recs.manager.Manager;

public class UpdateOrdersMessage implements Message{

	private static final long serialVersionUID = 3878724391031400944L;

	private long[][] orders;

	public UpdateOrdersMessage(long[][] orders) {
		this.orders = orders;
	}
	
	public long[][] getOrders() {
		return orders;
	}

	@Override
	public void handle() {
		Manager.getInstance().setOrders(orders);
	}
}
