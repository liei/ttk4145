package edu.ntnu.ttk4145.recs.manager;

import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.Arrays;
import java.util.HashMap;
import java.util.SortedMap;
import java.util.TreeMap;

import edu.ntnu.ttk4145.recs.Elevator;
import edu.ntnu.ttk4145.recs.Order;
import edu.ntnu.ttk4145.recs.Peer;
import edu.ntnu.ttk4145.recs.UpdateStateMessage;
import edu.ntnu.ttk4145.recs.Util;
import edu.ntnu.ttk4145.recs.driver.Driver.Button;
import edu.ntnu.ttk4145.recs.network.Radio;

public class Manager {
	
	private final static String MULTICAST_GROUP = "224.0.2.1";
	private final static int SEND_PORT = 7001;
	private final static int RECEIVE_PORT = 7002;
	private final static long MY_ID = Util.makeLocalId();
	private Peer master;
	private boolean isMaster = true;
	
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
		discoverMaster();
	}

	public void updatePeer(long id, long timeOfLastAlive, InetAddress address) {
		if(peers.containsKey(id)) {
			peers.get(id).setTimeOfLastAlive(timeOfLastAlive);
		}
		else {
			Peer newPeer = new Peer(address, id);
			peers.put(id, newPeer);
			if(newPeer.getId() < master.getId()) {
				master = newPeer;
			}
		}
	}
	
	public static void main(String[] args) throws SocketException {

		try {
			InetAddress ip = InetAddress.getLocalHost();
			System.out.println(Arrays.toString(ip.getAddress()));
		} catch (UnknownHostException e) {

			e.printStackTrace();
		}
		
		Radio radio = new Radio(MULTICAST_GROUP, SEND_PORT, RECEIVE_PORT);
		radio.start();
	}

	public static String getMulticastgroup() {
		return MULTICAST_GROUP;
	}

	public static int getSendport() {
		return SEND_PORT;
	}

	public void registerCall(Button button, int floor) {
		// TODO Auto-generated method stub
		
	}
	
	public void updateState(Elevator elevator){
		for(Peer peer : peers.values()){
			peer.sendMessage(new UpdateStateMessage(elevator));
		}
	}
	
	public void discoverMaster() {
		try {
			//Wait to receive a few alive messages before we search list of peers for a master.
			Thread.sleep(Radio.getAliveInterval());
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		long minId = MY_ID;
		//Peer with lowest ID is master.
		for (Peer peer : peers.values()) {
			if(minId < peer.getId()) {
				minId = peer.getId();
				isMaster = false;
			}
		}
		master = peers.get(minId);
	}
	
}
