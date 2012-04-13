package edu.ntnu.ttk4145.recs.manager;

import java.util.HashMap;
import java.util.Map;

import edu.ntnu.ttk4145.recs.Elevator;
import edu.ntnu.ttk4145.recs.driver.Driver.Button;

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

	public void updatePeer(int id, int tick) {
		Elevator peer = peers.get(id);
		peer.updateLife(tick);
	}

	public void registerCall(Button button, int floor) {
		// TODO Auto-generated method stub
		
	}
	
	
	
	
	
	
	
}
