package edu.ntnu.ttk4145.recs;

import edu.ntnu.ttk4145.recs.driver.Driver;
import edu.ntnu.ttk4145.recs.driver.Driver.Button;
import java.net.InetAddress;
import java.net.UnknownHostException;

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
			int id = 0;
			try {
				id = Util.intFromBytes(InetAddress.getLocalHost().getAddress());
			} catch (UnknownHostException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			localInstance = new Elevator(id);
		}
		return localInstance;
	}
	
	public int getId() {
		return id;
	}
	
	
	public Elevator(int id){
		this.id = id;
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
	
	
}
