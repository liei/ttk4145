package edu.ntnu.ttk4145.recs.driver;

import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.UIManager;

public class SimulatedDriver extends Driver{
	
	public static final Icon STOP_LAMP_ON = new ImageIcon("res/stop_on.png");  
	public static final Icon STOP_LAMP_OFF = new ImageIcon("res/stop_off.png");
	
	public static final Icon DOOR_LAMP_ON = new ImageIcon("res/door_on.png");
	public static final Icon DOOR_LAMP_OFF = new ImageIcon("res/door_off.png");
	
	private static final Icon FLOOR_LAMP_ON = new ImageIcon("res/floor_on.png");
	private static final Icon FLOOR_LAMP_OFF = new ImageIcon("res/floor_off.png");
	
	private static final Icon[] CALL_ICONS_OFF = {
		new ImageIcon("res/call_up_off.png"),
		new ImageIcon("res/call_down_off.png"),
		new ImageIcon("res/call_command_off.png")
	};
	
	private static final Icon[] CALL_ICONS_ON = {
		new ImageIcon("res/call_up_on.png"),
		new ImageIcon("res/call_down_on.png"),
		new ImageIcon("res/call_command_on.png")
	};
	
	private static final int[] FLOORS = {310,210,110,10};
	
	private int y = (int)(FLOORS[3] + (Math.random() * (FLOORS[0]-FLOORS[3])));
	private int speed = 0;
	
	private final StatePanel sp = new StatePanel();
	private final ButtonPanel bp = new ButtonPanel();
	
	private Thread thread;
	private boolean running;
	private DriverCallbacks callbacks = DriverCallbacks.NULL;
	
	private int prevf = 0;

	protected SimulatedDriver(){
		thread = new Thread(){
			public void run(){
				running = true;
				
				
				while(running){
					y -= speed;
					y = Math.min(FLOORS[0],y);
					y = Math.max(FLOORS[3],y);
					
					if(prevf != y){
						for(int f = 0; f < FLOORS.length; f++){
							if(prevf == FLOORS[f]){
								callbacks.floorSensorTriggered(f, false);
								break;
							}
							if(y == FLOORS[f]){
								callbacks.floorSensorTriggered(f, true);
								break;
							}
						}
					}
					
					prevf = y;
					
					sp.repaint();
					try {
						Thread.sleep(10);
					} catch (InterruptedException e) {}
				}
			}
		};
		thread.start();
		
		JFrame elevWindow = new JFrame();
		elevWindow.setContentPane(bp);
		elevWindow.setResizable(true);
		elevWindow.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		elevWindow.setResizable(false);
		elevWindow.setVisible(true);
		elevWindow.pack();
		
		JFrame buttonWindow = new JFrame();
		buttonWindow.setContentPane(sp);
		buttonWindow.setResizable(true);
		buttonWindow.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		buttonWindow.pack();
		buttonWindow.setVisible(true);
	}

	public void setSpeed(int speed){
		this.speed = (int) Math.signum(speed);
	}
	
	public void setDoorOpenLamp(boolean on) {
		bp.doorLamp.setIcon(on ? DOOR_LAMP_ON : DOOR_LAMP_OFF);
	}
	
	public void setStopLamp(boolean on) {
		bp.stopButton.setIcon(on ? STOP_LAMP_ON : STOP_LAMP_OFF);
	}
	
	public void setFloorIndicator(int floor) {
		for(JLabel label : sp.floorLamps){
			label.setIcon(FLOOR_LAMP_OFF);
		}
		sp.floorLamps[floor].setIcon(FLOOR_LAMP_ON);
		
	}

	public void setButtonLamp(Call call, int floor, boolean on) {
		bp.callButtons[call.ordinal()][floor].setIcon(on ? CALL_ICONS_ON[call.ordinal()] : CALL_ICONS_OFF[call.ordinal()]);
	}
	
	public int getFloorSensorState(){
		for(int f = 0; f < FLOORS.length; f++){
			if(y == FLOORS[f]){
				return f;
			}
		}
		return -1;
	}

	public void resetAllLamps() {
		setStopLamp(false);
		setDoorOpenLamp(false);
		for(int floor = 0; floor < NUMBER_OF_FLOORS; floor++){
			setButtonLamp(Call.DOWN,floor,false);
			setButtonLamp(Call.UP,floor,false);
			setButtonLamp(Call.COMMAND,floor,false);
		}
	}
	
	public void clearElevatorState(){
		resetAllLamps();
		setSpeed(0);
	}
	

	
	@Override
	public void startCallbacks(DriverCallbacks callbacks) {
		this.callbacks = callbacks;

		prevf = 0;
	}

	@Override
	public void stopCallbacks() {
		callbacks = DriverCallbacks.NULL;
	}
	
	class ButtonPanel extends JPanel{

		private static final long serialVersionUID = -2360436050981928050L;

		private JButton[][] callButtons = new JButton[Call.values().length][Driver.NUMBER_OF_FLOORS];
		
		private JLabel doorLamp = new JLabel(DOOR_LAMP_OFF);
		private JButton stopButton = new JButton(STOP_LAMP_OFF);
		
		private ButtonPanel(){
			
			setLayout(new GridBagLayout());

			GridBagConstraints gbc = new GridBagConstraints();
			
			for(int floor = 0; floor < Driver.NUMBER_OF_FLOORS; floor++){
				gbc.gridx = 0;
				gbc.gridy = floor;
				add(new JLabel(String.format("%d:",Driver.NUMBER_OF_FLOORS - floor)),gbc);
				for(Call call : Call.values()){
					int c = call.ordinal();

					
					JButton button = new JButton(CALL_ICONS_OFF[c]);
					callButtons[c][3-floor] = button;
					if((call == Call.DOWN && floor == 3) || (call == Call.UP && floor == 0)){
						continue;
					}
					final Call fCall = call;
					final int f = Driver.NUMBER_OF_FLOORS - floor - 1;
					button.addActionListener(new ActionListener(){
						@Override
						public void actionPerformed(ActionEvent ae) {
							callbacks.buttonPressed(fCall, f);
						}
					});
					gbc.gridx = c + 1;
					gbc.gridy = floor;
					
					add(button,gbc);
				}
			}
			
			stopButton.addActionListener(new ActionListener() {
				@Override
				public void actionPerformed(ActionEvent arg0) {
					callbacks.stopButtonPressed();
				}
			});
			gbc.gridy++;
			gbc.gridx = 1;
			
			
			add(stopButton,gbc);
			
			gbc.gridx++;
			
			JCheckBox obs = new JCheckBox("Obstructed");
			obs.addActionListener(new ActionListener() {
				@Override
				public void actionPerformed(ActionEvent e) {
					callbacks.obstructionSensorTriggered(((JCheckBox)e.getSource()).isSelected());
				}
			});
			add(obs,gbc);

			gbc.gridx++;
			add(doorLamp,gbc);
		}
	}
	
	private class StatePanel extends JPanel{
		
		private static final long serialVersionUID = 1264411218888490068L;

		
		private JLabel[] floorLamps = new JLabel[Driver.NUMBER_OF_FLOORS];
		
		private StatePanel(){
			
			setLayout(new GridBagLayout());
			
			GridBagConstraints gbc = new GridBagConstraints();
			
			gbc.gridx = 0;
			gbc.gridy = 0;
			
			gbc.gridheight = 4;
			
			add(new ElevatorPanel(),gbc);
			
			gbc.gridheight = 1;
			gbc.gridx = 1;
			for(int y = 0; y < Driver.NUMBER_OF_FLOORS; y++){
				gbc.gridy = y;
				JLabel floorLamp = new JLabel(FLOOR_LAMP_OFF);
				add(floorLamp,gbc);
				floorLamps[3-y] = floorLamp;
			}
		}
	}
	
	private class ElevatorPanel extends JPanel{

		private static final long serialVersionUID = 1L;

		private static final int X = 8;
		
		private BufferedImage bg;
		private BufferedImage elev;
		
		public ElevatorPanel(){
			try {
				bg = ImageIO.read(new File("res/bg.png"));
				elev = ImageIO.read(new File("res/elev.png"));
			} catch (IOException e) {
				e.printStackTrace();
			}
			setPreferredSize(new Dimension(bg.getWidth(),bg.getHeight()));
		}
		
		@Override
		public void paintComponent(Graphics g){
			g.drawImage(bg,0,0,null);
			g.drawImage(elev,X,y,null);
		}
	}
	
	public static void main(String[] args) {
		
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (Exception e) {}
		
		
		final Driver driver = Driver.makeInstance(SimulatedDriver.class);
		
		driver.startCallbacks(new DriverCallbacks() {
			
			boolean stop = false;

			@Override
			protected void stopButtonPressed() {
				driver.setStopLamp(stop = !stop);
				System.out.println("STOP!");
			}
			
			@Override
			protected void obstructionSensorTriggered(boolean enabled) {
				driver.setDoorOpenLamp(enabled);
				System.out.printf("obstruction(%b)%n",enabled);
			}
			
			@Override
			protected void floorSensorTriggered(int floor, boolean arriving) {
				driver.setFloorIndicator(floor);
				System.out.printf("floorSensor(%d,%b)%n",floor,arriving);
			}
			
			
			@Override
			protected void buttonPressed(Call call, int floor) {
				driver.setSpeed(call == Call.UP ? 1000 : -1000);
				driver.setButtonLamp(call, floor, true);
				System.out.printf("buttonPressed(%s,%d)%n",call,floor);
			}
		});
	}
}
