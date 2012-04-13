package edu.ntnu.ttk4145.recs.manager;

import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.util.SortedMap;
import java.util.TreeMap;

import edu.ntnu.ttk4145.recs.Elevator;
import edu.ntnu.ttk4145.recs.Order;
import edu.ntnu.ttk4145.recs.Peer;
import edu.ntnu.ttk4145.recs.UpdateStateMessage;
import edu.ntnu.ttk4145.recs.driver.Driver.Button;
import edu.ntnu.ttk4145.recs.network.Radio;

public class Manager {
	
	
	public HashMap<Integer,Order> orders;
	
	private static Manager instance;
	
	public static Manager getInstance() {
		if(instance == null){
			instance = new Manager();
		}
		return instance;
	}
	
	SortedMap<Long,Peer> peers;
	
	private Manager(){
		peers = new TreeMap<Long,Peer>();
	}

	public void updatePeer(int id, long time) {
		System.out.println("ID: " + id +" time: " + time);
	}
	
	public static void main(String[] args) throws SocketException {

		try {
			InetAddress ip = InetAddress.getLocalHost();
			System.out.println(Arrays.toString(ip.getAddress()));
		} catch (UnknownHostException e) {

			e.printStackTrace();
		}
		
		Radio radio = new Radio("224.0.2.1", 7001, 7002, "12");
		radio.start();
	}

	public void registerCall(Button button, int floor) {
		// TODO Auto-generated method stub
		
	}
	
	public void updateState(Elevator elevator){
		for(Peer peer : peers.values()){
			peer.sendMessage(new UpdateStateMessage(elevator));
		}
	}
	
	
	
	
	
	
	
}
