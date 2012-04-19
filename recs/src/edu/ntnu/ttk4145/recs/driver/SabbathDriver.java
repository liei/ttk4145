package edu.ntnu.ttk4145.recs.driver;

public class SabbathDriver extends Driver{

	@Override
	protected void buttonPressed(Call call, int floor) {
		// Do nothing
	}

	int speed = 500;
	
	@Override
	protected void floorSensorTriggered(int floor, boolean arriving) {
		if(arriving){
			setFloorIndicator(floor);
			setDoorOpenLamp(true);
			try {
				//let people in/on
				Thread.sleep(5000);
			} catch (InterruptedException e) {}
			setDoorOpenLamp(false);
		}
		if(floor == Driver.NUMBER_OF_FLOORS - 1){
			speed = -500;
		} else if(floor == 0){
			speed =  500;
		}
		updateElevator();
	}

	private boolean stopped = false;
	
	@Override
	protected void stopButtonPressed() {
		stopped = !stopped;
		updateElevator();
	}

	@Override
	protected void obstructionSensorTriggered(boolean enabled) {
		stopped = enabled;
		updateElevator();
	}
	
	private void updateElevator(){
		setStopLamp(stopped);
		setSpeed(stopped ? 0 : speed);
	}
}
