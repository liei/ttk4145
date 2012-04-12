package edu.ntnu.ttk4145.recs;

import java.net.InetAddress;

public class Elevator {

	
	private int id;
	private InetAddress address;
	
	private Direction dir;
	private boolean lamps[][];
	
	private int orders[][];
	
	private static Elevator localInstance = null;
	
	public static Elevator getLocalElevator(){
		if(localInstance == null){
			localInstance = new Elevator();
		}
		return localInstance;
	}
	
	
	
	public static enum Direction {
		UP, DOWN, STOP;
	}



	public void updateLife(int tick) {
		// TODO Auto-generated method stub
		
	}
	
}
