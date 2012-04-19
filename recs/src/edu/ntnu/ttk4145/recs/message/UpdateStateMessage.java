package edu.ntnu.ttk4145.recs.message;

import edu.ntnu.ttk4145.recs.Elevator;
import edu.ntnu.ttk4145.recs.manager.Manager;

public class UpdateStateMessage implements Message{

	private static final long serialVersionUID = 1684296774611243683L;

	private long peerId;
	private Elevator.State state;
	
	public UpdateStateMessage(long elevatorId, Elevator.State state) {
		this.state = state;
		this.peerId = elevatorId;
	}
	
	public Elevator.State getState(){
		return state;
	}
	
	public long getElevatorId() {
		return peerId;
	}

	@Override
	public void handle() {
		Manager.getInstance().updatePeerState(peerId, state);
	}
}
