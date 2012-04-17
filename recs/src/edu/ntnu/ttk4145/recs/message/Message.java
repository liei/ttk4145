package edu.ntnu.ttk4145.recs.message;

import java.io.Serializable;

public class Message implements Serializable{

	private static final long serialVersionUID = -1608844516979631070L;
	
	private Type type;
	
	public Message(Type type){
		this.type = type;
	}
	
	public Type getType(){
		return type;
	}

	public static enum Type{
		UPDATE_STATE, UPDATE_ORDERS, ORDER_DONE, NEW_ORDER;
	}
	
}
