package edu.ntnu.ttk4145.recs;

import edu.ntnu.ttk4145.recs.driver.Driver;
import edu.ntnu.ttk4145.recs.driver.Driver.Button;


public class Elevator {

	
	private int id;
	
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
			localInstance = new Elevator();
		}
		return localInstance;
	}
	
	public int getId() {
		return id;
	}
	
	
	
	public static enum Direction {
		UP, DOWN, STOP;
	}



	public void updateLife(int tick) {
		
	}

	public void addOrder(Order order){
		
	}

	public void addOrder(Button button, int floor) {
		
	}
	
}
