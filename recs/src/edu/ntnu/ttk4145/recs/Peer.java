package edu.ntnu.ttk4145.recs;

import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

import edu.ntnu.ttk4145.recs.manager.Manager;

public class Peer {

	
	private InetAddress ip;
	private long timeOfLastAlive;
	private long id;
	private DatagramSocket socket = null;
	
	public Peer(InetAddress address, long id) {
		ip = address;
		this.id = id;
		try {
			socket = new DatagramSocket(Manager.getSendport());
		} catch (SocketException e) {
			e.printStackTrace();
		}
	}
	
	
	public boolean sendMessage(Message msg){
		return true;
	}
	
	public void setTimeOfLastAlive(long timeOfLastAlive) {
		this.timeOfLastAlive = timeOfLastAlive;	
	}
	
	public long getId() {
		return id;
	}
}
