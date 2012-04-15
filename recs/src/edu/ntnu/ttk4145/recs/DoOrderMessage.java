package edu.ntnu.ttk4145.recs;

public class DoOrderMessage extends Message {

	private Order order;

	public DoOrderMessage(Order order) {
		super(Message.Type.DO_ORDER);
		this.order = order;
	}
	
	public Order getOrder() {
		return order;
	}

	/**
	 * 
	 */
	private static final long serialVersionUID = -4851917575955984284L;

}
