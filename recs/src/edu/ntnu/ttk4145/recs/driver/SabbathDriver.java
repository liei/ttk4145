package edu.ntnu.ttk4145.recs.driver;

public class SabbathDriver extends Driver{

	public static final int UP   =  250;
	public static final int DOWN = -250;
	
	@Override
	protected void buttonPressed(Call call, int floor) {
		// Do nothing
	}

	int speed;
	
	@Override
	public void setSpeed(int speed){
		this.speed = speed;
		super.setSpeed(speed);
	}
	
	@Override
	protected void floorSensorTriggered(int floor, boolean arriving) {
		if(arriving){
			setFloorIndicator(floor);
			setDoorOpenLamp(true);
			setSpeed(0);
			try {
				//let people in/on
				Thread.sleep(5000);
			} catch (InterruptedException e) {}
			setDoorOpenLamp(false);
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
	
	public static void main(String[] args) {
		Driver.makeInstance(SabbathDriver.class).startCallbacks().setSpeed(250);
	}
}
