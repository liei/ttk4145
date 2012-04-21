package edu.ntnu.ttk4145.recs.driver;

import java.io.IOException;

import edu.ntnu.ttk4145.recs.driver.Driver.Call;

public class TestCallbacks extends DriverCallbacks{

	@Override
	protected void buttonPressed(Call button, int floor) {
		Driver.getInstance().setButtonLamp(button,floor,true);
		System.out.printf("TestDriver.buttonPressed(%s,%d)%n",button,floor);
	}
	
	private boolean stop = false;
	
	@Override
	protected void stopButtonPressed() {
		Driver.getInstance().setStopLamp(stop = !stop);
		System.out.println("Stop button pushed");
	}
	
	@Override
	protected void floorSensorTriggered(int floor, boolean arriving) {
		Driver.getInstance().setFloorIndicator(floor);
		
		if(floor == 0){
			Driver.getInstance().setSpeed(1000);
		} else if (floor == 3){
			Driver.getInstance().setSpeed(-1000);
		}
		System.out.printf("Floor %d, %s%n",floor + 1,arriving ? "arrive" : "depart");
	}

	@Override
	protected void obstructionSensorTriggered(boolean enabled) {
		Driver.getInstance().setDoorOpenLamp(enabled);
		System.out.println("Obstruction switch flipped");
	}
	
	public static void main(String[] args) {
		Driver driver = Driver.makeInstance(SimulatedDriver.class);
		driver.resetAllLamps();
		driver.setSpeed(-1000);
		
		driver.startCallbacks(new TestCallbacks());
		System.out.println("Started (enter to quit)");
		try {
			System.in.read();
		} catch (IOException e) {}
		
		
		driver.stopCallbacks();
	}
}
