package edu.ntnu.ttk4145.recs.message;

import edu.ntnu.ttk4145.recs.Order;

public class OrderDoneMessage extends Message {

	private static final long serialVersionUID = -4851917575955984284L;

	private long elevId;
	private Order order;

	public OrderDoneMessage(long elevId, Order order) {
		super(Type.DONE);
		this.elevId = elevId;
		this.order = order;
	}
	
	public long getElevId(){
		return elevId;
	}
	
	public Order getOrder(){
		return order;
	}
}
