package edu.ntnu.ttk4145.recs.message;

import edu.ntnu.ttk4145.recs.Order;

public class NewOrderMessage extends Message {

	private static final long serialVersionUID = -6747782053298761561L;

	private Order order;
	
	public NewOrderMessage(Order order) {
		super(Type.NEW_ORDER);
		this.order = order;
	}

	public Order getOrder(){
		return order;
	}
}
