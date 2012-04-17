package edu.ntnu.ttk4145.recs.manager;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.SortedMap;
import java.util.TreeMap;

import edu.ntnu.ttk4145.recs.Elevator;
import edu.ntnu.ttk4145.recs.Elevator.Direction;
import edu.ntnu.ttk4145.recs.Order;
import edu.ntnu.ttk4145.recs.Peer;
import edu.ntnu.ttk4145.recs.Util;
import edu.ntnu.ttk4145.recs.driver.Driver;
import edu.ntnu.ttk4145.recs.driver.Driver.Call;
import edu.ntnu.ttk4145.recs.message.Message;
import edu.ntnu.ttk4145.recs.message.OrderDoneMessage;
import edu.ntnu.ttk4145.recs.message.UpdateOrdersMessage;
import edu.ntnu.ttk4145.recs.message.UpdateStateMessage;
import edu.ntnu.ttk4145.recs.network.Radio;

public class Manager {
	
	private static final String MULTICAST_GROUP = "224.0.2.1";
	private static final int SEND_PORT = 7001;
	private static final int RECEIVE_PORT = 7002;
	
	private static final long NO_ORDER = 0;
	
	private static Manager instance;
	
	public static Manager getInstance() {
		if(instance == null){
			instance = new Manager(Util.makeLocalId());
		}
		return instance;
	}
	
	private final long myId;

	public long[][] orders = new long[2][Driver.NUMBER_OF_FLOORS];
	
	SortedMap<Long,Peer> peers;
	private Peer master;
	
	private Manager(long id){
		myId = id;
		peers = new TreeMap<Long,Peer>();
	}
	
	public long getId() {
		return myId;
	}
	
	/**
	 * Starts the manager instance. The manager will sent and listen to alive messages.
	 * Send and receive orders to and from the local elevator.
	 */
	public void startManager(){
		Radio radio = new Radio(MULTICAST_GROUP, SEND_PORT, RECEIVE_PORT);
		radio.start();
		discoverMaster();		
		MessageListener peerListener = new MessageListener();
		peerListener.listen();
	}
	
	/**
	 * Handle a newly received alive message.
	 * 
	 * @param peerId The ID of the peer who sent the alive message.
	 * @param timeOfLastAlive The time the alive message was received.
	 * @param address The address of the peer.
	 */
	public synchronized void  handleAliveMessage(long peerId, long timeOfLastAlive, InetAddress address) {
		if(peers.containsKey(peerId)) {
			peers.get(peerId).updateAliveTime(timeOfLastAlive);
		}
		else {
			Peer newPeer = new Peer(address, peerId);
			peers.put(peerId, newPeer);
			if(newPeer.getId() < master.getId()) {
				master = newPeer;
			}
		}
	}
	
	/**
	 * Update the local representation peer's state.
	 * @param peerId The Id of the peer.
	 * @param newState The peer's new state.
	 */
	public void updatePeerState(long peerId, Elevator.State newState) {
		peers.get(peerId).updateState(newState);
	}
	
	/**
	 * 
	 * @return The multicast group to listen to for alive messages over UDP.
	 */
	public static String getMulticastgroup() {
		return MULTICAST_GROUP;
	}

	/**
	 * 
	 * @return The port used to send TCP and UDP messages.
	 */
	public static int getSendport() {
		return SEND_PORT;
	}
	
	/**
	 * 
	 * @return The port used to receive TCP and UDP messages.
	 */
	public static int getReciveport() {
		return RECEIVE_PORT;
	}

	/**
	 * Register a local call for elevator. Notice every peer that a call has been made.
	 * 
	 * @param button The button that was pressed.
	 * @param floor The floor where the button was pressed.
	 */
	public void registerCall(Call call, int floor) {
		orders[call.ordinal()][floor] = myId;
		System.out.printf("registerCall (%s,%d) = %d",call,floor,myId);
		Elevator.getLocalElevator().updateElevatorState();
		//Elevator.getLocalElevator().addOrder(order);
//		for (Peer peer : peers.values()) {
//			// TODO
////			peer.sendMessage(new OrderMessage(order));
//		}
	}
	
	/**
	 * Notify the peers about a change in elevator state.
	 * @param state The new elevator state.
	 */
	public void sendUpdateStateMessages(Elevator.State state){
		UpdateStateMessage updateStateMessage = new UpdateStateMessage(myId, state);
		for(Peer peer : peers.values()){
			 peer.sendMessage(updateStateMessage);
		}
	}
	
	/**
	 * Removes a peer from the list of all active peers.
	 * @param peer The peer who has timed out.
	 */
	public void removePeer(Peer peer) {
		peers.remove(peer);
		if(peer == master) {
			setMaster();
			if(master.getId() == myId) {
				redistributeOrders(peer);
			}
		}
	}
	
	/**
	 * This peer has died and his orders are distributed to the other peers.
	 * @param peer Dead peer.
	 */
	private void redistributeOrders(Peer peer) {
		for(int dir = 0; dir < orders.length; dir++){
			for(int floor = 0; floor < orders[dir].length; floor++){
				if(orders[dir][floor] == peer.getId()){
					Order order = new Order(Direction.values()[dir],floor);
					Peer best = findBestPeerForOrder(order);
					orders[order.getDir().ordinal()][order.getFloor()] = best.getId();
				}
			}
		}
		updatePeerOrders();
	}
	
	
	
	
	/**
	 * 
	 * Finds the peer best suited to perform the Order and dispatches the order.
	 * @param order An order to perform
	 */
	private void dispatchOrder(Order order){
		Peer best = findBestPeerForOrder(order);
		orders[order.getDir().ordinal()][order.getFloor()] = best.getId();
		updatePeerOrders();
	}
	
	private Peer findBestPeerForOrder(Order order){
		Peer bestPeer = null;
		double best = 1;
		for(Peer peer : peers.values()) {
			double value = evaluateStateForOrder(peer.getState(), order);
			if(value < best) {
				best = value; 
				bestPeer = peer;
			}
		}
		return bestPeer;
	}
	
	private double evaluateStateForOrder(Elevator.State state,Order order){
		return Math.random();
	}
	
	
	
	/**
	 * Listens to alive messages to register peers and finds out who's master.
	 */
	private void discoverMaster() {
		try {
			//Wait to receive a few alive messages before we search list of peers for a master.
			Thread.sleep(Radio.getAliveInterval());
		} catch (InterruptedException e) {}
		setMaster();
	}
	
	private void setMaster() {
		master = peers.get(peers.firstKey());
	}

	/**
	 * 
	 * A class for listening to messages from the other peers on the network.
	 * Spawns new threads to handle incoming messages.
	 *
	 */
	private class MessageListener {
		
		private ServerSocket server = null;
		
		public MessageListener() {
			try {
				server = new ServerSocket(RECEIVE_PORT);
			} catch (IOException e) {
				e.printStackTrace();
			}
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
	
	/**
	 * 
	 * A class to handle the messages received from other peers.
	 * The message types handled are of types ORDER, DO_ORDER,
	 * DONE and STATE.
	 *
	 */
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
				ois.close();
				socket.close();
			} catch (IOException e) {
				e.printStackTrace();
			} catch (ClassNotFoundException e) {
				e.printStackTrace();
			}
			handleMessage(message);
		}
		
		private void handleMessage(Message message) {
			switch(message.getType()) {
				case ORDERS: 
					UpdateOrdersMessage ordersMessage = (UpdateOrdersMessage) message; 
					setOrders(ordersMessage.getOrders());
					break;
				case STATE:
					UpdateStateMessage stateMessage = (UpdateStateMessage) message;
					peers.get(stateMessage.getElevatorId()).updateState(stateMessage.getState());
					break;
				case DONE:
					OrderDoneMessage doneMessage = (OrderDoneMessage) message;
					deleteOrder(doneMessage.getElevId(),doneMessage.getOrder());
					break;
				default:
					throw new RuntimeException("Unpossible!");
			}	
		}
	}
	
	private synchronized void deleteOrder(long elevId, Order order){
		Direction dir = order.getDir();
		int floor = order.floor;
		if(orders[dir.ordinal()][floor] == elevId){
			orders[dir.ordinal()][floor] = NO_ORDER;
		}
		updatePeerOrders();
	}

	private synchronized void updatePeerOrders() {
		long[][] ordersCopy = Util.copyOf(orders);
		for(Peer peer : peers.values()){
			peer.sendMessage(new UpdateOrdersMessage(ordersCopy));
		}
	}
	
	public synchronized long[][] getOrders() {
		return Util.copyOf(orders);
	}

	public synchronized void setOrders(long[][] orders) {
		this.orders = orders;
	}

	public synchronized void orderDone(Direction dir, int floor) {
		master.sendMessage(new OrderDoneMessage(myId,new Order(dir,floor)));
	}
}
