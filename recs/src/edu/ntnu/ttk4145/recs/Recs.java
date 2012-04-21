package edu.ntnu.ttk4145.recs;

import edu.ntnu.ttk4145.recs.driver.Driver;
import edu.ntnu.ttk4145.recs.driver.SimulatedDriver;
import edu.ntnu.ttk4145.recs.manager.Manager;

public class Recs {
	
	public static void main(String[] args) {
		Driver.makeInstance(SimulatedDriver.class);
		Elevator.getLocalElevator().init();
		Manager.getInstance().startManager();
	}
}
