package edu.ntnu.ttk4145.recs;

import edu.ntnu.ttk4145.recs.driver.Driver;
import edu.ntnu.ttk4145.recs.driver.SabbathDriver;
import edu.ntnu.ttk4145.recs.manager.Manager;

public class Recs {
	
	public static void main(String[] args) {
		
		Driver.makeInstance(SabbathDriver.class).startCallbacks();
		
		
//		Elevator.getLocalElevator().init();
//		Manager.getInstance().startManager();
	}
}
