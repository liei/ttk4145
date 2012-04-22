package edu.ntnu.ttk4145.recs.network;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.MulticastSocket;

import edu.ntnu.ttk4145.recs.Util;
import edu.ntnu.ttk4145.recs.elevator.Manager;
/**
 * 
 * The radio class is used to send and receive alive messages.
 * The radio sends and listens to broadcasts to and from other peers.
 * Two threads are spawned; one to listen and one to send. 
 */
public class Radio {
	
	private String multicastGroup;
	private int receivePort;
	private AliveSender sender;
	private AliveListener listener;
	private final static int ALIVE_INTERVAL = 100; //ms
	private final static int ALIVE_TIMEOUT  = 5 * ALIVE_INTERVAL;
	
	/**
	 * 
	 * @param multicastGroup The multicast group used for UDP broadcasts.
	 * @param sendPort The port to send UDP messages to.
	 * @param receivePort The port used to receive messages.
	 */
	public Radio(String multicastGroup, int receivePort) {
		this.receivePort = receivePort;
		this.multicastGroup = multicastGroup;
		sender = new AliveSender();
		listener = new AliveListener();
	}
	
	/**
	 * Starts the radio, spawns two threads: to listen and send alive messages.
	 */
	public void start() {
		new Thread(sender).start();
		new Thread(listener).start();
	}
	
	/**
	 * Stops listening to alive messages.
	 */
	public void stopAliveListener() {
		listener.stop();
	}
	
	/**
	 * Stop sending alive messages.
	 */
	public void stopAliveSender() {
		sender.stop();
	}
	
	/**
	 * 
	 * @return The interval between alive messages, in ms.
	 */
	public static int getAliveInterval() {
		return ALIVE_INTERVAL;
	}
	
	/**
	 * 
	 * @return The time until a peer is considered dead, timed out, in ms.
	 */
	public static int getAliveTimeout() {
		return ALIVE_TIMEOUT;
	}
	
	/**
	 * 
	 * Class used to listen for alive messages. Implements the runnable interface.
	 * Must be started in its own thread.
	 *
	 */
	private class AliveListener implements Runnable{
		
		private boolean running;
		private MulticastSocket socket;
		
		@Override
		public void run() {
			InetAddress group = null;
			try {
				socket = new MulticastSocket(receivePort);
				group = InetAddress.getByName(multicastGroup);
				socket.joinGroup(group);
			} catch (IOException ioe) {
				ioe.printStackTrace();
			}
			running = true;
			
			while(running){
				DatagramPacket packet;
				byte[] buffer = new byte[256];
				packet = new DatagramPacket(buffer, buffer.length);
				try {
					socket.receive(packet);
					long peerId = parseMessage(packet);
					Manager.getInstance().handleAliveMessage(peerId, System.currentTimeMillis(), packet.getAddress());
				} catch (IOException e) {
					continue;
				}
			}
			
			try{
				socket.leaveGroup(group);
				socket.close();
			} catch (IOException ioe) {
				ioe.printStackTrace();
			}
		}
		
		/**
		 * 
		 * @param packet The datagram packet containing an alive message.
		 * @return The Id of the peer who sent the alive message.
		 */
		private long parseMessage(DatagramPacket packet) {
			byte[] bytes = packet.getData();
			return Util.longFromBytes(bytes);
		}
		
		/**
		 * Stop this thread.
		 */
		public void stop() {
			running = false;
		}
	}
	
	/**
	 * 
	 * Class used to broadcast alive messages over UDP.
	 * Implements the runnable interface. Must be started
	 * in its own thread. Broadcasts an alive messages every
	 * ALIVE_INTERVAL ms.
	 *
	 */
	private class AliveSender implements Runnable {
		
		private boolean running;
		private DatagramSocket socket;

		public void run() {
			InetAddress group = null;
			try {
				socket = new DatagramSocket();
				group = InetAddress.getByName(multicastGroup);
				running = true;
			} catch (IOException ioe) {
				System.err.printf("Failed to start AliveSender: %s%n",ioe.getMessage());
			}
			while(running) {
				long myId = Manager.getInstance().getId();
				byte[] aliveMessage = Util.asBytes(myId);
				try {
					socket.send(new DatagramPacket(aliveMessage, aliveMessage.length, group, receivePort));
				} catch (IOException e) {}
				
				try {
					Thread.sleep(ALIVE_INTERVAL);
				} catch (InterruptedException e) {}
			}
		}
		
		public void stop() {
			running = false;
		}
	}
}
