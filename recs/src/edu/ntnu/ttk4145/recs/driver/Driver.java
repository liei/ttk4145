package edu.ntnu.ttk4145.recs.driver;

public abstract class Driver {

	public static final int NUMBER_OF_FLOORS = 4;
	
	private static Driver instance = null;
	
	public static Driver makeInstance(Class<? extends Driver> driverClass){
		try {
			return instance = driverClass.newInstance();
		} catch (InstantiationException e) {
			throw new IllegalArgumentException(e);
		} catch (IllegalAccessException e) {
			throw new IllegalArgumentException(e);
		}
	}
	
	public static Driver getInstance(){
		if(instance == null){
			throw new IllegalStateException("Instance is null, call Driver.makeInstance first.");
		}
		return instance;
	}
	
	public abstract void setSpeed(int speed);
	
	public abstract void setDoorOpenLamp(boolean on);
	
	public abstract void setStopLamp(boolean on);
	
	public abstract void setFloorIndicator(int floor);

	public abstract void setButtonLamp(Call call, int floor, boolean on);
	
	public abstract int getFloorSensorState();

	public abstract void resetAllLamps();
	
	public abstract void clearElevatorState();
	
	public abstract void startCallbacks(DriverCallbacks callbacks);
	
	public abstract void stopCallbacks();
		
	public static enum Call {
		UP, DOWN, COMMAND;
	}
}
