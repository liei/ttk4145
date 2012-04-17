package edu.ntnu.ttk4145.recs;

import edu.ntnu.ttk4145.recs.Elevator.Direction;

public class Order {

	public final Direction dir;
	public final int floor;
	
	public Order(Direction dir, int floor){
		this.dir = dir;
		this.floor = floor;
	}

	public Direction getDir() {
		return dir;
	}

	public int getFloor() {
		return floor;
	}
	

}
