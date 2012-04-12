package edu.ntnu.ttk4145.recs.alive;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;

import edu.ntnu.ttk4145.recs.manager.Manager;

public class AliveListener implements Runnable{

	public static final String MULTICAST_GROUP = "224.0.0.5";
	
	private boolean running;
	private MulticastSocket socket;
	
	@Override
	public void run() {
		InetAddress group = null;
		try {
			socket = new MulticastSocket(4446);
			group = InetAddress.getByName(MULTICAST_GROUP);
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
				int[] msg = parseMessage(packet);
				Manager.getInstance().updatePeer(msg[0],msg[0]);
			} catch (IOException e) {
				continue;
			}
			
			String received = new String(packet.getData());
			System.out.println("Quote of the Moment: " + received);
		}
		
		try{
			socket.leaveGroup(group);
			socket.close();
		} catch (IOException ioe) {
			ioe.printStackTrace();
		}
	}

	private int[] parseMessage(DatagramPacket packet) {
		// TODO Auto-generated method stub
		return null;
	}
}
