package edu.ntnu.ttk4145.recs;

import java.io.Serializable;

import edu.ntnu.ttk4145.recs.Elevator.Direction;

public class Order implements Serializable {

	private static final long serialVersionUID = -1942998659992463832L;

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
