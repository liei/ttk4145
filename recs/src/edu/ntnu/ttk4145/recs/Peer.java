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
	
	/**
	 * 
	 * @param msg
	 * @return true if the peer is alive.
	 */
	public boolean sendMessage(Message msg){
		if(hasTimedOut()) {
			return false;
		}
		try {
			messageStream.writeObject(msg);
			return true;
		} catch (IOException e) {
			e.printStackTrace();
		}
				
		return true;
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
	
}
