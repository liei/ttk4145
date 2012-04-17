package edu.ntnu.ttk4145.recs.message;

import edu.ntnu.ttk4145.recs.Elevator.Direction;

public class OrderDoneMessage extends Message {

	private static final long serialVersionUID = -4851917575955984284L;

	private Direction dir;
	private int floor;
	private long elevId;
	
	public OrderDoneMessage(Direction dir, int floor,long elevId) {
		super(Type.DONE);
		this.dir = dir;
		this.floor = floor;
		this.elevId = elevId;
	}
	
	public long getElevId(){
		return elevId;
	}
	
	public Direction getDir(){
		return dir;
	}
	
	public int getFloor(){
		return floor;
	}
	
	
}
