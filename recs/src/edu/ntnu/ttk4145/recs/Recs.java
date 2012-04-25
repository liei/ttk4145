package edu.ntnu.ttk4145.recs;

import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintStream;

import org.apache.commons.cli.BasicParser;
import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;

import edu.ntnu.ttk4145.recs.driver.Driver;
import edu.ntnu.ttk4145.recs.driver.JniDriver;
import edu.ntnu.ttk4145.recs.driver.RecsCallbacks;
import edu.ntnu.ttk4145.recs.driver.SabbathCallbacks;
import edu.ntnu.ttk4145.recs.driver.SimulatedDriver;
import edu.ntnu.ttk4145.recs.elevator.Elevator;
import edu.ntnu.ttk4145.recs.elevator.Manager;

public class Recs {
	
	private static final Options options = new Options();
	private static final String USAGE = "java -jar recs.jar [-sfq] [-i <ni>]";
	
	static {
		options.addOption("s","simulation",false,"Run simulated elevator")
				.addOption("f","sabbath",false,"Run in sabbath mode")
				.addOption("q","quiet",false,"Don't print anything")
				.addOption("i","interface",true,"What network interface to use")
				.addOption("?",false,"Don't print anything")
				.addOption("h","help",false,"Don't print anything");
	}
	
	
	public static void main(String[] args) {
		
		CommandLine cl = null;
		CommandLineParser clp = new BasicParser();
		HelpFormatter hf = new HelpFormatter();
		
		try {
			cl = clp.parse(options,args);
		} catch (ParseException e) {
			e.printStackTrace();
			hf.printHelp(USAGE,options);
			System.exit(1);
	    }

		if(cl.hasOption('h') || cl.hasOption('?')){
			hf.printHelp(USAGE,options);
			System.exit(0);
	    }
	    
		if(cl.hasOption('i')){
			Util.setNI(cl.getOptionValue('i'));
		}
		
		
		if(cl.hasOption('q')){
			PrintStream nullPs = new PrintStream(new OutputStream(){
				@Override
				public void write(int arg0) throws IOException {
					// do nothing!
				}
			});
			System.setOut(nullPs);
			System.setErr(nullPs);
		}

		Driver driver = Driver.makeInstance(cl.hasOption('s') ? SimulatedDriver.class : JniDriver.class);
		Elevator.getLocalElevator().init();
		driver.startCallbacks(cl.hasOption('f') ? new SabbathCallbacks() : new RecsCallbacks());
		Manager.getInstance().startManager();
	}
}
