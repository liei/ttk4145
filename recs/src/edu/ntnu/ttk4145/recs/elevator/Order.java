package edu.ntnu.ttk4145.recs.elevator;

import java.io.Serializable;

import edu.ntnu.ttk4145.recs.elevator.Elevator.Direction;

public class Order implements Serializable {

	private static final long serialVersionUID = -1942998659992463832L;

	public final Direction dir;
	public final int floor;
	
	public Order(Direction dir, int floor){
		this.dir = dir;
		this.floor = floor;
	}

	public Direction getDirection() {
		return dir;
	}

	public int getFloor() {
		return floor;
	}
}
