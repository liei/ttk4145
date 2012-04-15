package edu.ntnu.ttk4145.recs;

import static edu.ntnu.ttk4145.recs.Order.NO_ORDER;
import edu.ntnu.ttk4145.recs.driver.Driver;
import edu.ntnu.ttk4145.recs.driver.Driver.Call;
import edu.ntnu.ttk4145.recs.manager.Manager;

public class Elevator {

	private static Elevator localInstance = null;
	
	public static Elevator getLocalElevator(){
		if(localInstance == null){
			localInstance = new Elevator(Util.makeLocalId());
		}
		return localInstance;
	}

	private long id;

	private State state;
	
	public Elevator(long id){
		this.id = id;
	}
	
	public long getId() {
		return id;
	}
	
	public boolean isStopped(){
		return state.stopped;
	}
	
	public Direction getDirection() {
		return state.dir;
	}

	public void setDirection(Direction dir) {
		state.dir = dir;
	}

	public boolean isObstructed() {
		return state.obstructed;
	}

	public void setObstructed(boolean obstructed) {
		state.obstructed = obstructed;
	}

	public boolean isDoorsOpen() {
		return state.doorsOpen;
	}

	public void setDoorsOpen(boolean doorsOpen) {
		state.doorsOpen = doorsOpen;
	}

	public int getFloor() {
		return state.floor;
	}

	public void setFloor(int floor, boolean arriving) {
		if(arriving){
			state.floor = floor;
			state.atFloor = false;
		} else {
			state.atFloor = true;
		}
		updateElevatorState();
	}

	public void setStopped(boolean stopped) {
		state.stopped = stopped;
		updateElevatorState();
	}
	
	public void addOrder(Order order){
		state.orders[order.call.ordinal()][order.floor] = order.id;
	}

	public void addOrder(Call button, int floor) {
		state.orders[button.ordinal()][floor] = 1;
	}
	
	private void updateElevatorState(){
		
		if(state.stopped || state.obstructed){
			state.dir = Direction.NONE;
		} else if(state.atFloor) {
			// Stopped at a floor
			long orderId = state.orders[state.dir.ordinal()][state.floor];
			if(orderId != NO_ORDER || state.orders[Call.COMMAND.ordinal()][state.floor] != NO_ORDER){
				// Stop at this floor;
				letPeopleOnOff();
				
				if(orderId != NO_ORDER){
					Manager.getInstance().orderDone(orderId);
				}
				
				state.orders[state.dir.ordinal()][state.floor] = NO_ORDER;
				state.orders[Call.COMMAND.ordinal()][state.floor] = NO_ORDER;
			} else {
				state.atFloor = false;
			}
		} else {
			
		}
		
		int callsOver = 0;
		int callsUnder = 0;
		
		for(Call call : Call.values()){
			for(int floor = 0; floor < Driver.NUMBER_OF_FLOORS; floor++){
				if(state.orders[call.ordinal()][floor] != NO_ORDER){
					if(floor > state.floor){
						callsOver++;
					} else if (floor < state.floor){
						callsUnder++;
					}
				}
			}
		}
		
		
		if(callsOver == 0){
			if(callsUnder == 0){
				
			} else {
				
			}
		} else {
			if(callsUnder == 0){
				
			} else {
				
			}
		}
		
		updateElevatorHardware();
		Manager.getInstance().updateState();
	}
	
	private void letPeopleOnOff() {
		// TODO Auto-generated method stub
		
	}

	private void updateElevatorHardware() {
		Driver driver = Driver.getInstance();
		for(Call button : Call.values()){
			for(int floor = 0; floor < Driver.NUMBER_OF_FLOORS; floor++){
				driver.setButtonLamp(button, floor, state.orders[button.ordinal()][floor] != 0);
			}
		}
		driver.setStopLamp(state.stopped);
		driver.setDoorOpenLamp(state.doorsOpen);
		driver.setFloorIndicator(state.floor);
		if(state.atFloor){
			driver.setSpeed(0);
		} else {
			driver.setSpeed(state.dir.speed);
		}
	}

	public static enum Direction {
		UP(1000), DOWN(-1000), NONE(0);
		
		private int speed;
		
		private Direction(int speed){
			this.speed = speed;
		}
	}
	
	public static class State{
		
		private Direction dir;
		
		private long[][] orders = new long[Call.values().length][Driver.NUMBER_OF_FLOORS];
		
		private boolean atFloor;
		private boolean stopped;
		private boolean obstructed;
		private boolean doorsOpen;
		
		private int floor;
	}

	public State getState() {
		// TODO Auto-generated method stub
		return null;
	}
}
