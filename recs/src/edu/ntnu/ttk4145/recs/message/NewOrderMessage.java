package edu.ntnu.ttk4145.recs.message;

import edu.ntnu.ttk4145.recs.elevator.Manager;
import edu.ntnu.ttk4145.recs.elevator.Order;

public class NewOrderMessage implements Message {

	private static final long serialVersionUID = -6747782053298761561L;

	private Order order;
	
	public NewOrderMessage(Order order) {
		this.order = order;
	}

	public Order getOrder(){
		return order;
	}
	
	public void handle() {
		Manager.getInstance().addOrder(order);
	}
}
