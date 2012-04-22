package edu.ntnu.ttk4145.recs.network;

import java.io.IOException;
import java.io.ObjectOutputStream;
import java.net.InetAddress;
import java.net.Socket;

import edu.ntnu.ttk4145.recs.elevator.Elevator;
import edu.ntnu.ttk4145.recs.elevator.Manager;
import edu.ntnu.ttk4145.recs.message.Message;

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
	
	public long getId() {
		return id;
	}
	
	public InetAddress getInetAddress() {
		return ip;
	}
	
	/**
	 * Update the state for this peer, returns true if this peer
	 * needs to have it's orders redistributed.
	 * @param newState
	 * @return
	 */
	public boolean updateAndEvaluateState(Elevator.State newState) {
		boolean redistribute = false;
		if(state != null && !state.isStopped() && newState.isStopped()){
			redistribute = true;
		}
		
		this.state = newState;
		return redistribute;
	}
	
	public Elevator.State getState() {
		return state;
	}
	
	public void updateAliveTime(long timeOfLastAlive) {
		if(timeOfLastAlive > this.timeOfLastAlive) {
			this.timeOfLastAlive = timeOfLastAlive;	
		}
	}
	
	public boolean hasTimedOut() {
		return (timeOfLastAlive + Radio.getAliveTimeout()) < System.currentTimeMillis();
	}
	
	
	public boolean sendMessage(final Message msg){
		if(hasTimedOut()){
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
					System.err.println("Could not send message to peer.");
				}
			}
		}.start();
		return true;
	}
}
