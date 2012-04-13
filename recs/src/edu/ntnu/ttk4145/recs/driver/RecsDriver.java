package edu.ntnu.ttk4145.recs.driver;

import edu.ntnu.ttk4145.recs.Elevator;
import edu.ntnu.ttk4145.recs.manager.Manager;

public class RecsDriver extends Driver {

	@Override
	protected void buttonPressed(Button button, int floor) {
		
		switch(button){
		case UP:
		case DOWN:
			Manager manager = Manager.getInstance();
			manager.registerCall(button,floor);
			break;
		case COMMAND:
			Elevator elev = Elevator.getLocalElevator();
			elev.addOrder(button,floor);
			break;
		}
	}

	@Override
	protected void floorSensorTriggered(int floor, boolean arriving) {
		Elevator elev = Elevator.getLocalElevator();
	}

	@Override
	protected void stopButtonPressed() {
		Elevator elev = Elevator.getLocalElevator();
		
	}

	@Override
	protected void obstructionSensorTriggered(boolean enabled) {
		// TODO Auto-generated method stub
		
	}

}
