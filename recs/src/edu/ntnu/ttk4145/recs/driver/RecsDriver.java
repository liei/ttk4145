package edu.ntnu.ttk4145.recs.driver;

import edu.ntnu.ttk4145.recs.Elevator;
import edu.ntnu.ttk4145.recs.manager.Manager;

public class RecsDriver extends Driver {

	@Override
	protected void buttonPressed(Call call, int floor) {
		
		Manager manager = Manager.getInstance();
		switch(call){
		case UP:
			manager.registerCall(call, floor);
		case DOWN:
			manager.registerCall(call,floor);
			break;
		case COMMAND:
			Elevator elev = Elevator.getLocalElevator();
			elev.addOrder(call,floor);
			break;
		}
	}

	@Override
	protected void floorSensorTriggered(int floor, boolean arriving) {
		Elevator elev = Elevator.getLocalElevator();
		elev.setFloor(floor,arriving);
	}

	@Override
	protected void stopButtonPressed() {
		Elevator elev = Elevator.getLocalElevator();
		elev.setStopped(true);
	}

	@Override
	protected void obstructionSensorTriggered(boolean enabled) {
		Elevator elev = Elevator.getLocalElevator();
		elev.setObstructed(enabled);
	}

}
