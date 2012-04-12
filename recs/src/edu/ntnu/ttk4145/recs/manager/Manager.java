package edu.ntnu.ttk4145.recs.manager;

import java.util.HashMap;
import java.util.Map;

import edu.ntnu.ttk4145.recs.Elevator;

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
	
	
	
	
	
	
	
}
