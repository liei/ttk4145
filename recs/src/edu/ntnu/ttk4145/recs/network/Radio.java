package edu.ntnu.ttk4145.recs.network;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.nio.ByteBuffer;

import edu.ntnu.ttk4145.recs.Elevator;
import edu.ntnu.ttk4145.recs.Util;
import edu.ntnu.ttk4145.recs.manager.Manager;
public class Radio {
	
	private String multicastGroup;
	private String aliveMessage;
	private int receivePort;
	private int sendPort;
	private AliveSender sender;
	AliveListener listener;

	public Radio(String multicastGroup, int sendPort, int receivePort, String aliveMessage) {
		this.aliveMessage = aliveMessage;
		this.sendPort = sendPort;
		this.receivePort = receivePort;
		this.multicastGroup = multicastGroup;
		sender = new AliveSender();
		listener = new AliveListener();
	}
	
	public void start() {
		new Thread(sender).start();
		new Thread(listener).start();
	}
	
	public void stopAliveListener() {
		listener.stop();
	}
	
	public void stopAliveSender() {
		sender.stop();
	}
	
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
				byte[] buf = new byte[256];
				packet = new DatagramPacket(buf, buf.length);
				try {
					socket.receive(packet);
					int id = parseMessage(packet);
					Manager.getInstance().updatePeer(id, System.currentTimeMillis());
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

		private int parseMessage(DatagramPacket packet) {
			byte[] bytes = packet.getData();
			return ByteBuffer.wrap(bytes).getInt();
		}
		
		public void stop() {
			running = false;
		}
	}
	
	private class AliveSender implements Runnable {
		
		private boolean running;
		private DatagramSocket socket;
		
		public void run() {
			InetAddress group = null;
			try {
				socket = new DatagramSocket(sendPort);
				group = InetAddress.getByName(multicastGroup);
			} catch (IOException ioe) {
				ioe.printStackTrace();
			}
			running = true;
			while(running) {
				int id = Elevator.getLocalElevator().getId();
				byte[] msg = Util.bytesFromInt(id);
				try {
					socket.send(new DatagramPacket(msg, msg.length, group, receivePort));
				} catch (IOException e) {
					continue;
				}
				try {
					Thread.sleep(1000);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					continue;
				}
			}
		}
		
		public void stop() {
			running = false;
		}
	}

}
