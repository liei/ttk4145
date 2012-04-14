package edu.ntnu.ttk4145.recs;

import edu.ntnu.ttk4145.recs.driver.Driver.Call;

public class Order {

	public static final int NO_ORDER = 0;
	
	private static int counter = 0;
	
	public final long id;
	public final Call call;
	public final int floor;
	
	public Order(Call call, int floor){
		this.id = (Elevator.getLocalElevator().getId() & 0xff00) + (counter++);
		this.call = call;
		this.floor = floor;
	}
}
