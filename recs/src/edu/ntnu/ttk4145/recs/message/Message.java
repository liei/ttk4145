package edu.ntnu.ttk4145.recs.message;

import java.io.Serializable;

public interface Message extends Serializable{
	
	public void handle();
	
}
