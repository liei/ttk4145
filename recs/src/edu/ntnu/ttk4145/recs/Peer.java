package edu.ntnu.ttk4145.recs;

import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.Socket;
import java.net.SocketException;
import java.util.LinkedList;
import java.util.List;
import java.io.BufferedOutputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;

import edu.ntnu.ttk4145.recs.driver.Driver.Call;
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
	
	public List<Order> getOrders() {
		List<Order> orderList = new LinkedList<Order>();
		long[][] orders = state.getOrders();
		for(Call call : Call.values()){
			for (int floor = 0; floor < orders[0].length; floor++) {
				long id = orders[call.ordinal()][floor];
				if(id != Order.NO_ORDER) {
					orderList.add(new Order(call, floor, id));
				}
			}
		}
		return orderList;
	}
	/**
	 * 
	 * @param order The order to evaluate
	 * @return Double in rage [0,1] indicating how easy it would be for this peer
	 * to perform the order. 
	 */
	public double getOrderRating(Order order) {
		// TODO Auto-generated method stub
		return 1;
	}
	
}
