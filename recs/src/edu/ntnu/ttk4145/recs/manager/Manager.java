package edu.ntnu.ttk4145.recs.manager;

import java.net.InetAddress;
import java.util.SortedMap;
import java.util.TreeMap;

import edu.ntnu.ttk4145.recs.Elevator;
import edu.ntnu.ttk4145.recs.Elevator.Direction;
import edu.ntnu.ttk4145.recs.Order;
import edu.ntnu.ttk4145.recs.Peer;
import edu.ntnu.ttk4145.recs.Util;
import edu.ntnu.ttk4145.recs.driver.Driver;
import edu.ntnu.ttk4145.recs.driver.Driver.Call;
import edu.ntnu.ttk4145.recs.message.MessageListener;
import edu.ntnu.ttk4145.recs.message.NewOrderMessage;
import edu.ntnu.ttk4145.recs.message.OrderDoneMessage;
import edu.ntnu.ttk4145.recs.message.UpdateOrdersMessage;
import edu.ntnu.ttk4145.recs.message.UpdateStateMessage;
import edu.ntnu.ttk4145.recs.network.Radio;

public class Manager {
	
	private static final String MULTICAST_GROUP = "224.0.2.1";
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
	
	// orders[Direction.UP.ordinal()][4] == peer.id means there's an order for the elevator to move to
	// floor 5 to pick up someone who wants to go up.   (..ordinal() = 0)
	// orders[1][7] == peer.id means someone on floor 8 wants to go down and peer has to pick them up.
	public long[][] orders = new long[2][Driver.NUMBER_OF_FLOORS];
	
	SortedMap<Long,Peer> peers;
	
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
		MessageListener.startListener(RECEIVE_PORT);
		
		Radio radio = new Radio(MULTICAST_GROUP, RECEIVE_PORT);
		radio.start();
		

		while(true){
			try {
				Thread.sleep(Radio.getAliveTimeout());
			} catch (InterruptedException e) {}
			updatePeers();
		}
	}
	
	/**
	 * Handle a newly received alive message.
	 * 
	 * @param peerId The ID of the peer who sent the alive message.
	 * @param timeOfLastAlive The time the alive message was received.
	 * @param address The address of the peer.
	 */
	public synchronized void  handleAliveMessage(long peerId, long timeOfLastAlive, InetAddress address) {
		Peer peer = peers.get(peerId);
		if(peer == null) {
			peer = new Peer(address, peerId);
			peers.put(peerId, peer);
		}
		peer.updateAliveTime(timeOfLastAlive);
	}
	
	/**
	 * Update the local representation peer's state.
	 * @param peerId The Id of the peer.
	 * @param newState The peer's new state.
	 */
	public synchronized void updatePeerState(long peerId, Elevator.State newState) {
		Peer peer = peers.get(peerId);
		if(peer != null){
			if(peer.updateAndEvaluateState(newState) && isMaster()){
				redistributeOrders(peer);
			}
		} else {
			System.err.printf("No peer with peerId: %d, disregard updateMessage.",peerId);
		}
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
	 * @return The port used to receive TCP and UDP messages.
	 */
	public static int getReciveport() {
		return RECEIVE_PORT;
	}

	/**
	 * Register a local call for elevator. Send message to master.
	 * 
	 * @param button The button that was pressed.
	 * @param floor The floor where the button was pressed.
	 */
	public synchronized void registerCall(Call call, int floor) {
		System.out.printf("registerCall (%s,%d)%n",call,floor);
		getMaster().sendMessage(new NewOrderMessage(new Order(Direction.values()[call.ordinal()],floor)));
	}
	
	private Peer getMaster() {
		return peers.get(peers.firstKey());
	}

	/**
	 * Notify the peers about a change in elevator state.
	 * @param state The new elevator state.
	 */
	public synchronized void sendUpdateStateMessages(Elevator.State state){
		UpdateStateMessage updateStateMessage = new UpdateStateMessage(myId, state);
		for(Peer peer : peers.values()){
			 peer.sendMessage(updateStateMessage);
		}
	}
	
	/**
	 * This peer has died and his orders are distributed to the other peers.
	 * @param peer Dead peer.
	 */
	public synchronized void redistributeOrders(Peer peer) {
		if(!isMaster()){
			return;
		}
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
	
	/**
	 * Finds the beer best suited to service an order.
	 * @param order The order to service.
	 * @return The peer best suited to carry out Order.
	 */
	private Peer findBestPeerForOrder(Order order){
		Peer bestPeer = null;
		double bestOm = -1;
		for(Peer peer : peers.values()) {
			double om = calculateOrderMatch(peer, order);
			if(om > bestOm) {
				bestOm = om; 
				bestPeer = peer;
			}
			if(bestOm > 0.95) break;
		}
		return bestPeer;
	}
	
	/**
	 * Used to evaluate how good a match this order is with this peer. 
	 * @param peer The peer to evaluate.
	 * @param order The order to be serviced.
	 * @return A double expressing how good a match this order is for the given peer.
	 * Where 0 indicates that this peer can't possibly service this order.
	 */
	private double calculateOrderMatch(Peer peer, Order order){
		Elevator.State state = peer.getState();
		
		if(state.isObstructed() || state.isStopped()) {
			return 0.0;
		}
		
		double orderBacklog = 0;

//		for (int floor = 0; floor < Driver.NUMBER_OF_FLOORS; floor++) {
//			if(orders[Direction.UP.ordinal()][floor] == peer.getId()) {
//				orderBacklog += ;
//			} 
//				
//			if(orders[Direction.DOWN.ordinal()][floor] == peer.getId()){
//				if(order.floor <)
//				
//				orderBacklog += ;
//			}
//		}
		
		for (int direction = 0; direction < 2; direction++) {
			for (int floor = 0; floor < Driver.NUMBER_OF_FLOORS; floor++) {
				if(orders[direction][floor] == peer.getId()) {
					orderBacklog++;
				}
			}
		}
		
		for (boolean internalCommand : state.getCommands()) {
			if(internalCommand) {
				orderBacklog++;
			}
		}
		if(orderBacklog == 0) {
			return 1.0;
		}
		
		double elevatorDirOppositeOrder = 0;
		if(state.getFloor() < order.getFloor() && state.getDirection() != Elevator.Direction.UP) {
			elevatorDirOppositeOrder = 1d;
		}
		else if(state.getFloor() > order.getFloor() && state.getDirection() != Elevator.Direction.DOWN) {
			elevatorDirOppositeOrder = 1d;
		}
		
		if(elevatorDirOppositeOrder == 1.0) {
			return 1.0;
		}
		
		double numFloorScalingFactor = 1.0 / Driver.NUMBER_OF_FLOORS;
		
		return (orderBacklog + elevatorDirOppositeOrder) * numFloorScalingFactor;
	}
	
	private boolean isMaster() {
		return myId == getMaster().getId();
	}
	
	public synchronized void deleteOrder(long elevId, Order order){
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
		Elevator.getLocalElevator().updateElevatorState();
	}

	public synchronized void orderDone(Direction dir, int floor) {
		getMaster().sendMessage(new OrderDoneMessage(myId,new Order(dir,floor)));
	}

	public synchronized void addOrder(Order order) {
		if(!isMaster()){
			getMaster().sendMessage(new NewOrderMessage(order));
		} else {
			dispatchOrder(order);
		}
	}

	private synchronized void updatePeers() {
		for(Peer peer : peers.values()){
			if(peer.hasTimedOut()){
				redistributeOrders(peer);
			}
		}
	}
}