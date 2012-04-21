package edu.ntnu.ttk4145.recs.driver;

import edu.ntnu.ttk4145.recs.driver.Driver.Call;

public class SabbathCallbacks extends DriverCallbacks{

	public static final int UP   =  250;
	public static final int DOWN = -250;
	
	@Override
	protected void buttonPressed(Call call, int floor) {
		// Do nothing
	}

	int speed = DOWN;
	
	@Override
	protected void floorSensorTriggered(int floor, boolean arriving) {
		System.out.printf("floorSensor(%d,%b)%n",floor,arriving);
		Driver driver = Driver.getInstance();
		if(arriving){
			driver.setFloorIndicator(floor);
			driver.setDoorOpenLamp(true);
			driver.setSpeed(0);
			try {
				//let people in/on
				Thread.sleep(5000);
			} catch (InterruptedException e) {}
			driver.setDoorOpenLamp(false);
		}
		if(floor == Driver.NUMBER_OF_FLOORS - 1){
			speed = DOWN;
		} else if(floor == 0){
			speed = UP;
		}
		updateElevator();
	}

	private boolean stopped = false;
	
	@Override
	protected void stopButtonPressed() {
		System.out.printf("stopButton()%n");
		stopped = !stopped;
		updateElevator();
	}

	@Override
	protected void obstructionSensorTriggered(boolean enabled) {
		System.out.printf("obstruction(%b)%n",enabled);
		stopped = enabled;
		updateElevator();
	}
	
	private void updateElevator(){
		Driver driver = Driver.getInstance();
		driver.setStopLamp(stopped);
		driver.setSpeed(stopped ? 0 : speed);
	}
	
	public static void main(String[] args) {
		Driver.makeInstance(SimulatedDriver.class).startCallbacks(new SabbathCallbacks());
		Driver.getInstance().setSpeed(-250);
	}
}
