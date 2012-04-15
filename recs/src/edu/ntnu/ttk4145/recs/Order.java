package edu.ntnu.ttk4145.recs;

import edu.ntnu.ttk4145.recs.driver.Driver.Call;

public class Order {

	public static final long NO_ORDER = 0;
	
	private static int counter = 0;
	
	public final long id;
	public final Call call;
	public final int floor;
	
	public Order(Call call, int floor){
		this(call,floor,Elevator.getLocalElevator().getId() & 0xff00 + counter++);
	}
	
	public Order(Call call, int floor, long id) {
		this.call = call;
		this.floor = floor;
		this.id = id;
	}

	public long getId() {
		return this.id;
	}

	public Call getCall() {
		return call;
	}

	public int getFloor() {
		return floor;
	}
	

}
