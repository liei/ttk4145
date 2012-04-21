package edu.ntnu.ttk4145.recs.driver;

import edu.ntnu.ttk4145.recs.driver.Driver.Call;

public abstract class DriverCallbacks {

	public static final DriverCallbacks NULL = new DriverCallbacks() {
		protected void stopButtonPressed() {}
		protected void obstructionSensorTriggered(boolean enabled) {}
		protected void floorSensorTriggered(int floor, boolean arriving) {}
		protected void buttonPressed(Call call, int floor) {}
	};
	
	protected abstract void buttonPressed(Call call, int floor);
	
	protected abstract void floorSensorTriggered(int floor, boolean arriving);
	
	protected abstract void stopButtonPressed();
	
	protected abstract void obstructionSensorTriggered(boolean enabled);
	
}
