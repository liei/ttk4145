package edu.ntnu.ttk4145.recs;

public class OrderMessage extends Message{

	private Order order;

	public OrderMessage(Order order) {
		super(Message.Type.ORDER);
		this.order = order;
	}
	
	public Order getOrder() {
		return order;
	}

	/**
	 * 
	 */
	private static final long serialVersionUID = 3878724391031400944L;


}
