package edu.ntnu.ttk4145.recs.message;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class MessageListener extends Thread{

	private ServerSocket server;
	private boolean running;

	private MessageListener(int port) {
		try {
			server = new ServerSocket(port);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void run() {
		running = true;
		while(running) {
			try {
				handleMessage(server.accept());
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	private void handleMessage(final Socket socket) {
		new Thread(){
			public void run() {
				Message message = null;
				try {
					ObjectInputStream ois = new ObjectInputStream(socket.getInputStream());
					message = (Message)ois.readObject();
					ois.close();
					socket.close();
				} catch (IOException e) {
					e.printStackTrace();
				} catch (ClassNotFoundException e) {
					e.printStackTrace();
				}
				message.handle();
			}
		}.start();
	}
	
	public static MessageListener startListener(int port){
		MessageListener ml = new MessageListener(port);
		ml.start();
		return ml;
	}
}

