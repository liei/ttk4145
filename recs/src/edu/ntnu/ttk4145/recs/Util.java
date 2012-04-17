package edu.ntnu.ttk4145.recs;

import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.Enumeration;
import java.util.Random;

public class Util {
	
	private static final Random RNG = new Random();

	public static int intFromBytes(byte[] bytes){
		return ByteBuffer.wrap(bytes).getInt();
	}
	
	public static long longFromBytes(byte[] bytes){
		return ByteBuffer.wrap(bytes).getLong();
	}

	public static byte[] asBytes(long l) {
		return ByteBuffer.allocate(8).putLong(l).array();
	}
	
	public static byte[] asBytes(int i) {
		return ByteBuffer.allocate(4).putInt(i).array();
	}
	
	public static byte[] getLocalIp() {
	    byte[] ip = null;
		try {
			Enumeration<NetworkInterface> nis = NetworkInterface.getNetworkInterfaces();
			while(nis.hasMoreElements()) {
				ip = findNoLoopbackAddress(nis.nextElement());
			}
		} catch (SocketException e) {}
		
		if(ip == null){
			try {
				return InetAddress.getLocalHost().getAddress();
			} catch (UnknownHostException e) {}
		}
		return ip;
	}
	
	public static byte[] getLocalIp(String niName){
		byte[] ip = null;
		try {
			ip = findNoLoopbackAddress(NetworkInterface.getByName(niName));
		} catch (SocketException e) {}
		
		if(ip == null){
			try {
				return InetAddress.getLocalHost().getAddress();
			} catch (UnknownHostException e) {}
		}
		return ip;
	}

	private static byte[] findNoLoopbackAddress(NetworkInterface ni) {
		for (Enumeration<InetAddress> ips = ni.getInetAddresses(); ips.hasMoreElements();) {
			InetAddress ip = ips.nextElement();
			if (!ip.isLoopbackAddress()) {
				return ip.getAddress();
			}
		}
		return null;
	}

	public static long makeLocalId() {
		byte[] bytes = new byte[8];
		System.arraycopy(getLocalIp(), 0, bytes,0, 4);
		System.arraycopy(asBytes(RNG.nextInt()), 0, bytes, 4, 4);
		return longFromBytes(bytes);
	}

	public static long[][] copyOf(long[][] m) {
		long[][] c = new long[m.length][];
		for(int i = 0; i < c.length; i++){
			c[i] = Arrays.copyOf(m[i], m[i].length);
		}
		return c;
	}
}
