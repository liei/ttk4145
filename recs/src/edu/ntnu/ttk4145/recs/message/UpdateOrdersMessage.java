package edu.ntnu.ttk4145.recs.message;

public class UpdateOrdersMessage extends Message{

	private static final long serialVersionUID = 3878724391031400944L;

	private long[][] orders;

	public UpdateOrdersMessage(long[][] orders) {
		super(Type.ORDERS);
		this.orders = orders;
	}
	
	public long[][] getOrders() {
		return orders;
	}
}
