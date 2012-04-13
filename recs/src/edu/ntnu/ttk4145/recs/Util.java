package edu.ntnu.ttk4145.recs;

import java.nio.ByteBuffer;

public class Util {

	
	
	public static int intFromBytes(byte[] bytes){
		return ByteBuffer.wrap(bytes).getInt();
	}

	public static byte[] bytesFromInt(int i) {
		return ByteBuffer.allocate(4).putInt(i).array();
	}
	
}
