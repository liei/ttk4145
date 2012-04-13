package edu.ntnu.ttk4145.recs.manager;

import java.util.HashMap;
import java.util.Map;

import edu.ntnu.ttk4145.recs.Elevator;
import edu.ntnu.ttk4145.recs.driver.Driver.Button;
import edu.ntnu.ttk4145.recs.network.Radio;

public class Manager {
	
	
	

	private static Manager instance;
	
	public static Manager getInstance() {
		if(instance == null){
			instance = new Manager();
		}
		return instance;
	}
	
	Map<Integer,Elevator> peers;
	
	private Manager(){
		peers = new HashMap<Integer,Elevator>();
	}

	public void updatePeer(int id, long time) {
		Elevator peer = peers.get(id);
		System.out.println("ID: " + id +" time: " + time);
		//peer.updateLife(time);
	}
	
	public static void main(String[] args) {
		Radio radio = new Radio("225.1.1.1", 7001, 7002, "12");
		radio.start();
	}

	public void registerCall(Button button, int floor) {
		// TODO Auto-generated method stub
		
	}
	
	
	
	
	
	
	
}
