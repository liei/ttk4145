package edu.ntnu.ttk4145.recs;

import static edu.ntnu.ttk4145.recs.Order.NO_ORDER;

import java.io.PrintWriter;
import java.io.StringWriter;

import edu.ntnu.ttk4145.recs.driver.Driver;
import edu.ntnu.ttk4145.recs.driver.Driver.Call;
import edu.ntnu.ttk4145.recs.driver.RecsDriver;
import edu.ntnu.ttk4145.recs.manager.Manager;

public class Elevator {

	protected static final int DOOR_WAIT_TIME = 5; // number of seconds the door is open, in seconds

	protected static final long ONE_SECOND = 1000; // millis in one second

	private static Elevator localInstance = null;
	
	public static Elevator getLocalElevator(){
		if(localInstance == null){
			localInstance = new Elevator(Util.makeLocalId());
		}
		return localInstance;
	}

	private long id;

	private State state = new State();
	
	private Elevator(long id){
		this.id = id;
	}

	public void init(){
		Driver driver = Driver.makeInstance(RecsDriver.class);
		while(driver.getFloorSensorState() != 0){
			driver.setSpeed(-1000);
			try {
				Thread.sleep(10);
			} catch (InterruptedException e) {}
		}
		driver.setSpeed(0);
		driver.startCallbacks();
	}
	
	public long getId() {
		return id;
	}
	
	public Elevator.State getState() {
		return state;
	}
	
	public void setObstructed(boolean obstructed) {
		state.obstructed = obstructed;
		updateElevatorState();
	}

	public void setFloor(int floor, boolean arriving) {
		if(arriving){
			state.floor = floor;
		}
		state.atFloor = arriving;
		updateElevatorState();
	}

	public void setStopped(boolean stopped) {
		state.stopped = stopped;
		updateElevatorState();
	}
	
	public synchronized void addCommand(int floor) {
		state.commands[floor] = true;
		updateElevatorState();
	}
	
	private synchronized void updateElevatorState(){
		
		if(state.stopped || state.obstructed || state.doorsOpen){
			;
		} else {
			long[][] orders = Manager.getInstance().getOrders();
			boolean callsOver  = orders[Direction.UP.ordinal()][state.floor] == id;
			boolean callsUnder = orders[Direction.DOWN.ordinal()][state.floor] == id;
			for(int i = 0; i < orders.length; i++){
				for(int floor = 0; floor < Driver.NUMBER_OF_FLOORS; floor++){
					if(orders[i][floor] == id || state.commands[floor]){
						if(floor > state.floor){
							callsOver = true;
						} else if (floor < state.floor){
							callsUnder = true;
						} 
					}
				}
			}
			
			if(callsOver && !callsUnder){
				state.dir = Direction.UP;
			} else if(callsUnder && !callsOver){
				state.dir = Direction.DOWN;
			}
			
			if(state.atFloor) {
				// Stopped at a floor
				
				long orderId = orders[state.dir.ordinal()][state.floor];
				
				if(orderId == id || state.commands[state.floor]){
					// Stop at this floor;
					letPeopleOnOff();
					
					if(orderId != NO_ORDER){
						Manager.getInstance().orderDone(orderId);
					}
					
					// TODO: reset orders in manager
					Manager.getInstance().orderDone(state.dir,state.floor);
					state.commands[state.floor] = false;
				}
			}
			

			if(!callsOver && !callsUnder){
				state.dir = Direction.NONE;
			} else if (callsOver && callsUnder){
				state.dir = Direction.UP;
				state.atFloor = false;
			} else if (callsUnder && !callsOver){
				state.dir = Direction.DOWN;
				state.atFloor = false;
			} else {
				state.dir = state.dir; // noop, continue the way you were going.
				state.atFloor = false;
			}
		}
		
		System.out.println(state);
		
		updatePhysicalElevator();
		Manager.getInstance().sendUpdateStateMessages(state);
	}
	
	private void letPeopleOnOff() {
		state.doorsOpen = true;
		new Thread(new Runnable(){
			@Override
			public void run() {
				int wait = DOOR_WAIT_TIME;
				while(wait-- > 0 || state.obstructed){
					try {
						Thread.sleep(ONE_SECOND);
					} catch (InterruptedException e) {}
				}
				state.doorsOpen = false;
				updateElevatorState();
			}
		}).start();
	}

	private void updatePhysicalElevator() {
		Driver driver = Driver.getInstance();
		for(int floor = 0; floor < Driver.NUMBER_OF_FLOORS; floor++){
			driver.setButtonLamp(Call.COMMAND, floor, state.commands[floor]);
		}
		driver.setStopLamp(state.stopped);
		driver.setDoorOpenLamp(state.doorsOpen);
		driver.setFloorIndicator(state.floor);
		if(state.atFloor || state.doorsOpen || state.stopped || state.obstructed){
			driver.setSpeed(0);
		} else {
			driver.setSpeed(state.dir.speed);
		}
	}

	public static enum Direction {
		UP(100), DOWN(-100), NONE(0);
		
		private int speed;
		
		private Direction(int speed){
			this.speed = speed;
		}
	}
	
	public static class State{
		
		private Direction dir = Direction.DOWN;
		
		private int floor = -1;
		
		private boolean[] commands = new boolean[Driver.NUMBER_OF_FLOORS];

		private boolean atFloor = false;
		private boolean stopped = false;
		private boolean obstructed = false;
		private boolean doorsOpen = false;
		
		public Direction getDirection(){
			return dir;
		}
		
		public int getFloor(){
			return floor;
		}
		
		public boolean[] getCommands(){
			return commands;
		}
		
		public boolean isAtFloor(){
			return atFloor;
		}
		
		public boolean isStopped(){
			return stopped;
		}
		
		public boolean isObstructed(){
			return obstructed;
		}
		
		public boolean isDoorsOpen(){
			return doorsOpen;
		}
		
		public String toString(){
			StringWriter sw = new StringWriter();
			PrintWriter  pw = new PrintWriter(sw);
			
			pw.printf("dir: %s, floor: %d, [at:%c,s:%c,o:%c,d:%c]%n",dir,floor,
					atFloor ? '*':' ',stopped ? '*':' ',obstructed ? '*':' ',doorsOpen ? '*':' ');
			
			for(int floor = Driver.NUMBER_OF_FLOORS - 1; floor >= 0; floor--){
				pw.printf("%d: ",floor+1);
				pw.printf("%s: %d, ",commands[floor] ? '-' : ' ');
				pw.println();
			}
			return sw.toString();
		}
	}
}
