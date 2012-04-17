package edu.ntnu.ttk4145.recs.message;

import edu.ntnu.ttk4145.recs.Elevator;
import edu.ntnu.ttk4145.recs.Elevator.State;

public class UpdateStateMessage extends Message{

	private static final long serialVersionUID = 1684296774611243683L;

	private Elevator.State state;

	private long elevatorId;
	
	public UpdateStateMessage(long elevatorId, Elevator.State state) {
		super(Type.STATE);
		this.state = state;
		this.elevatorId = elevatorId;
	}
	
	public Elevator.State getState(){
		return state;
	}
	
	public long getElevatorId() {
		return elevatorId;
	}
}
