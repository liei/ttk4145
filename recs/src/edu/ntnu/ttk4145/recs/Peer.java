package edu.ntnu.ttk4145.recs;

import java.io.IOException;
import java.io.ObjectOutputStream;
import java.net.InetAddress;
import java.net.Socket;

import edu.ntnu.ttk4145.recs.manager.Manager;
import edu.ntnu.ttk4145.recs.message.Message;
import edu.ntnu.ttk4145.recs.network.Radio;

public class Peer {
	
	private long id;
	private InetAddress ip;
	private long timeOfLastAlive;
	private Elevator.State state;
	
	public Peer(InetAddress address, long id) {
		ip = address;
		this.id = id;
		try {
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public boolean sendMessage(final Message msg){
		if(hasTimedOut()) {
			Manager.getInstance().removePeer(this);
			return false;
		}
		new Thread(){
			public void run(){
				try {
					Socket socket = new Socket(ip, Manager.getReciveport());
					ObjectOutputStream oos = new ObjectOutputStream(socket.getOutputStream());
					oos.writeObject(msg);
					oos.close();
					socket.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}.start();
		return true;
	}
	
	public void updateAliveTime(long timeOfLastAlive) {
		if(timeOfLastAlive > this.timeOfLastAlive) {
			this.timeOfLastAlive = timeOfLastAlive;	
		}
	}
	
	private boolean hasTimedOut() {
		return (timeOfLastAlive + Radio.getAliveTimeout()) < System.currentTimeMillis();
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
