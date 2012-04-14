package edu.ntnu.ttk4145.recs;

import edu.ntnu.ttk4145.recs.driver.Driver;
import edu.ntnu.ttk4145.recs.driver.Driver.Button;
import java.net.InetAddress;
import java.net.UnknownHostException;

public class Elevator {

	
	private long id;
	
	private Direction dir;
	
	private boolean[][] lamps;
	
	private Order[][] orders = new Order[Button.values().length][Driver.NUMBER_OF_FLOORS];
	
	private boolean stopped;
	private boolean moving;
	private boolean obstructed;
	
	private double floor;
	
	private static Elevator localInstance = null;
	
	public static Elevator getLocalElevator(){
		if(localInstance == null){
			localInstance = new Elevator(Util.makeLocalId());
		}
		return localInstance;
	}
	
	public Elevator(long id){
		this.id = id;
	}
	
	public long getId() {
		return id;
	}
	
	
	
	public static enum Direction {
		UP, DOWN, STOP;
	}




	public void addOrder(Order order){
		
	}

	public void addOrder(Button button, int floor) {
		
	}
	
	public void updateLife(long time) {
		// TODO Auto-generated method stub
		
	}
	
	public Elevator.State getState() {
		return new State();
	}
	
	public static class State {
		
	}
	
//	Thread elevator;
//	private boolean running;
//	
//	public void startLocalElevatorThread(){
//		
//		elevator = new Thread(new Runnable(){
//			@Override
//			public void run() {
//				running = true;
//				while(running){
//					
//				}
//			}
//		});
//	}
//	
//	public void stopLocalElevatorThread(){
//		running = false;
//		try {
//			elevator.join();
//		} catch (InterruptedException e) {}
//	}
	
}
