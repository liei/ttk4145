package edu.ntnu.ttk4145.recs.message;

import edu.ntnu.ttk4145.recs.Order;
import edu.ntnu.ttk4145.recs.manager.Manager;

public class OrderDoneMessage implements Message{

	private static final long serialVersionUID = -4851917575955984284L;

	private long peerId;
	private Order order;

	public OrderDoneMessage(long elevId, Order order) {
		this.peerId = elevId;
		this.order = order;
	}
	
	public long getElevId(){
		return peerId;
	}
	
	public Order getOrder(){
		return order;
	}

	public void handle() {
		Manager.getInstance().deleteOrder(peerId,order);		
	}
}
