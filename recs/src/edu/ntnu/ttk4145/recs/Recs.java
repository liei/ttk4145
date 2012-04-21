package edu.ntnu.ttk4145.recs;

import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintStream;

import edu.ntnu.ttk4145.recs.driver.Driver;
import edu.ntnu.ttk4145.recs.driver.SimulatedDriver;
import edu.ntnu.ttk4145.recs.manager.Manager;

public class Recs {
	
	private static boolean debug = false;
	
	public static void main(String[] args) {
		
		if(!debug){
			PrintStream nullPs = new PrintStream(new OutputStream(){
				@Override
				public void write(int arg0) throws IOException {
					// do nothing!
				}
			});
			System.setOut(nullPs);
			System.setErr(nullPs);
		}
		
		
		Driver.makeInstance(SimulatedDriver.class);
		Elevator.getLocalElevator().init();
		Manager.getInstance().startManager();
	}
}
