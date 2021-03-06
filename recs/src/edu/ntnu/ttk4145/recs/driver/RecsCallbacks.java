package edu.ntnu.ttk4145.recs.driver;

import edu.ntnu.ttk4145.recs.driver.Driver.Call;
import edu.ntnu.ttk4145.recs.elevator.Elevator;
import edu.ntnu.ttk4145.recs.elevator.Manager;

public class RecsCallbacks extends DriverCallbacks {

	@Override
	protected void buttonPressed(Call call, int floor) {
		System.out.printf("RecsDriver.buttonPressed(%s,%d)%n",call,floor);
		Manager manager = Manager.getInstance();
		switch(call){
		case UP:
			manager.registerCall(call,floor);
			break;
		case DOWN:
			manager.registerCall(call,floor);
			break;
		case COMMAND:
			Elevator elev = Elevator.getLocalElevator();
			elev.addCommand(floor);
			break;
		}
	}

	@Override
	protected void floorSensorTriggered(int floor, boolean arriving) {
		System.out.printf("RecsDriver.floorSensorTriggered(%d,%b)%n",floor,arriving);
		Elevator elev = Elevator.getLocalElevator();
		elev.setFloor(floor,arriving);
	}

	@Override
	protected void stopButtonPressed() {
		System.out.println("RecsDriver.stopButtonPressed()");
		Elevator elev = Elevator.getLocalElevator();
		elev.setStopped(!elev.getState().isStopped());
	}

	@Override
	protected void obstructionSensorTriggered(boolean enabled) {
		System.out.printf("RecsDriver.obstructionSensorTriggered(%b)%n",enabled);
		Elevator elev = Elevator.getLocalElevator();
		elev.setObstructed(enabled);
	}
}
