package edu.ntnu.ttk4145.recs;

import edu.ntnu.ttk4145.recs.Elevator.Direction;

public class Order {

	private static int counter = 0;
	
	public final long id;
	public final Direction dir;
	public final int floor;
	
	public Order(Direction dir, int floor){
		this.id = (Elevator.getLocalElevator().getId() & 0xff00) + counter++;
		this.dir = dir;
		this.floor = floor;
	}
}
