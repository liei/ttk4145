package edu.ntnu.ttk4145.recs;

public class UpdateStateMessage extends Message{

	private static final long serialVersionUID = 1684296774611243683L;

	private Elevator state;
	
	public UpdateStateMessage(Elevator state) {
		super(Type.STATE);
		this.state = state;
	}
	
	public Elevator getState(){
		return state;
	}
}
