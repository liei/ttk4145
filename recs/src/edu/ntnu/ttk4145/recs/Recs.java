package edu.ntnu.ttk4145.recs;

import edu.ntnu.ttk4145.recs.driver.Driver;
import edu.ntnu.ttk4145.recs.driver.RecsDriver;
import edu.ntnu.ttk4145.recs.manager.Manager;

public class Recs {

	
	
	
	public static void main(String[] args) {
		
		Elevator.getLocalElevator().init();
		
		Manager.getInstance();
	
	}
}
