package edu.ntnu.ttk4145.recs.manager;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.Arrays;
import java.util.HashMap;
import java.util.SortedMap;
import java.util.TreeMap;

import edu.ntnu.ttk4145.recs.Elevator;
import edu.ntnu.ttk4145.recs.Message;
import edu.ntnu.ttk4145.recs.Order;
import edu.ntnu.ttk4145.recs.OrderMessage;
import edu.ntnu.ttk4145.recs.Peer;
import edu.ntnu.ttk4145.recs.UpdateStateMessage;
import edu.ntnu.ttk4145.recs.Util;
import edu.ntnu.ttk4145.recs.driver.Driver.Call;
import edu.ntnu.ttk4145.recs.network.Radio;

public class Manager {
	
	private final static String MULTICAST_GROUP = "224.0.2.1";
	private final static int SEND_PORT = 7001;
	private final static int RECEIVE_PORT = 7002;
	private final long myId = Util.makeLocalId();
	private Peer master;
	
	public HashMap<Long,Order> orders;
	
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
		Radio radio = new Radio(MULTICAST_GROUP, SEND_PORT, RECEIVE_PORT);
		radio.start();
		MessageListener peerListener = new MessageListener();
		peerListener.listen();
		discoverMaster();
	}

	public void updatePeer(long id, long timeOfLastAlive, InetAddress address) {
		if(peers.containsKey(id)) {
			peers.get(id).updateAliveTime(timeOfLastAlive);
		}
		else {
			Peer newPeer = new Peer(address, id);
			peers.put(id, newPeer);
			if(newPeer.getId() < master.getId()) {
				master = newPeer;
			}
		}
	}
	
	public void updatePeer(long peerId, Elevator.State newState) {
		peers.get(peerId).updateState(newState);
	}
	
	private void addOrder(Order order) {
		orders.put(order.getId(), order);
	}
	
	private void removeOrder(Order order) {
		orders.remove(order.getId());
	}
	

	public static String getMulticastgroup() {
		return MULTICAST_GROUP;
	}

	public static int getSendport() {
		return SEND_PORT;
	}
	
	public static int getReciveport() {
		return RECEIVE_PORT;
	}

	public void registerCall(Call button, int floor) {
		// TODO: Implement this right!
		Elevator.getLocalElevator().addOrder(new Order(button, floor));
	}
	
	public void updateState(Elevator.State state){
		for(Peer peer : peers.values()){
			peer.sendMessage(new UpdateStateMessage(myId, state));
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

		master = peers.get(peers.firstKey());
	}

	public void orderDone(long orderId) {
		System.out.printf("Order done: %l",orderId);
	}
	
	private class MessageListener {
		
		private ServerSocket server = null;
		public MessageListener() {
			try {
				server = new ServerSocket(RECEIVE_PORT);
			} catch (IOException e) {
				e.printStackTrace();
			}
			listen();
		}
		
		private void listen() {
			while(true) {
				MessageHandler handler;
				try {
					handler = new MessageHandler(server.accept());
					Thread t = new Thread(handler);
					t.start();
				} catch (Exception e) {
				}
			}
		}
	}
	
	private class MessageHandler implements Runnable{
		private Socket socket;
		
		public MessageHandler(Socket socket) {
			this.socket = socket;
		}
		@Override
		public void run() {
			ObjectInputStream ois = null;
			Message message = null;
			try {
				ois = new ObjectInputStream(socket.getInputStream());
				message = (Message)ois.readObject();
			} catch (IOException e) {
				e.printStackTrace();
			} catch (ClassNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			handleMessage(message);
		}
		private void handleMessage(Message message) {
			switch(message.getType()) {
				case ORDER: 
					Manager.getInstance().addOrder(((OrderMessage) message).getOrder());
					break;
				case DONE:
					Manager.getInstance().removeOrder(((OrderMessage) message).getOrder());
					break;
				case STATE:
					UpdateStateMessage stateMessage = (UpdateStateMessage) message;
					peers.get(stateMessage.getElevatorId()).updateState(stateMessage.getState());
					break;
				default:
					throw new RuntimeException("Unpossible!");
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
}
