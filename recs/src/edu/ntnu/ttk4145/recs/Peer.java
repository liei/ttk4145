package edu.ntnu.ttk4145.recs;

import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.Socket;
import java.net.SocketException;
import java.io.BufferedOutputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;

import edu.ntnu.ttk4145.recs.manager.Manager;
import edu.ntnu.ttk4145.recs.network.Radio;

public class Peer {

	
	private InetAddress ip;
	private long timeOfLastAlive;
	private long id;
	private Socket socket = null;
	ObjectOutputStream messageStream = null;
	private Elevator.State state;
	
	public Peer(InetAddress address, long id) {
		ip = address;
		this.id = id;
		try {
			socket = new Socket(ip, Manager.getReciveport());
			messageStream = new ObjectOutputStream(socket.getOutputStream());
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public void sendMessage(Message msg){
		if(hasTimedOut()) {
			Manager.getInstance().removePeer(this);
		}
		try {
			messageStream.writeObject(msg);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public void updateAliveTime(long timeOfLastAlive) {
		if(timeOfLastAlive > this.timeOfLastAlive) {
			this.timeOfLastAlive = timeOfLastAlive;	
		}
	}
	
	private boolean hasTimedOut() {
		if(timeOfLastAlive + Radio.getAliveTimeout() < System.currentTimeMillis()) {
			return true;
		}
		return false;
	}
	
	public long getId() {
		return id;
	}
	
	public InetAddress getInetAddress() {
		return ip;
	}
	
	public void updateState(Elevator.State newState) {
		this.state = newState;
	}
	
	public Elevator.State getState() {
		return state;
	}
	
}