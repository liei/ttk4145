package edu.ntnu.ttk4145.recs;

import java.net.InetAddress;
import java.net.UnknownHostException;

public class Elevator {

	
	private int id;
	
	private Direction dir;
	private boolean lamps[][];
	
	private int orders[][];
	
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
	
	public Elevator(int id){
		this.id = id;
	}
	
	public static enum Direction {
		UP, DOWN, STOP;
	}



	public void updateLife(long time) {
		// TODO Auto-generated method stub
		
	}
	
	public int getId() {
		return this.id;
	}
	
}
