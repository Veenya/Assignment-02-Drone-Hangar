package iot.drone_carrier;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.LayoutManager;
import java.awt.RenderingHints;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.LinkedList;

import javax.swing.*;

class DashboardView extends JPanel implements ActionListener  {

	// Send commands from GUI to Hangar simulating:
	private JButton landButton;      // - Landing
	private JButton takeOffButton;   // - Taking off

	private JButton alarmButton;     // Alarm simulation
	private boolean alarmActive = false;


	//private JButton reset;
	//private JButton openDoor; //todo: rivedere il meccanismo di come funziona

	// Show state of the drone (rest, taking off, operating, landing)
    private JTextField hangarTemperature;
	// Show state of the hangar (normal, alarm, prealarm)
	private JTextField hangarState;      

	// When landing, the distance from the ground
	private JTextField groundDistance;
	
	//private JTextField doorState;           // stato della porta (aperta, chiusa, movimento)
	// temperature of hangar
	private JTextField currentTemperature; 

	private int WINDOW_WIDTH;
	private int WINDOW_HEIGHT; 
	private int FIELD_WIDTH;
	private int FIELD_HEIGHT;

	
	
	private DashboardController controller;	
	
	public DashboardView(){
		//super(".:: Drone Carrier ::.");
		WINDOW_WIDTH = 600;
		WINDOW_HEIGHT = 150;
		FIELD_WIDTH = 200;
		FIELD_HEIGHT = 15;

		

		setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
		//this.setResizable(false);
		JPanel mainPanel = new JPanel();
		mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.Y_AXIS));
		mainPanel.add(Box.createRigidArea(new Dimension(0,20)));
		
		JPanel infoLine = new JPanel();
		infoLine.setLayout(new BoxLayout(infoLine, BoxLayout.LINE_AXIS));

		// Hangar State
		hangarState = new JTextField("--");
		hangarState.setEditable(false);
		hangarState.setPreferredSize(new Dimension(FIELD_WIDTH,FIELD_HEIGHT));
		infoLine.add(new JLabel("Hangar State:")); 
		infoLine.add(hangarState);

		// Drone State
		hangarTemperature = new JTextField("--");
		hangarTemperature.setEditable(false);
		hangarTemperature.setPreferredSize(new Dimension(FIELD_WIDTH,FIELD_HEIGHT));
		infoLine.add(new JLabel("Drone State:")); 
		infoLine.add(hangarTemperature);

		// Ground Distance
		groundDistance = new JTextField("--");
		groundDistance.setEditable(false);
		groundDistance.setPreferredSize(new Dimension(FIELD_WIDTH,FIELD_HEIGHT));
		infoLine.add(new JLabel("Ground Distance:")); 
		infoLine.add(groundDistance);

		// Temperature of hangar
		currentTemperature = new JTextField("--");
		currentTemperature.setEditable(false);
		currentTemperature.setPreferredSize(new Dimension(FIELD_WIDTH,FIELD_HEIGHT));
		infoLine.add(new JLabel("Current Temperature:"));
		infoLine.add(currentTemperature);
		
		/*
		// Door state
		doorState = new JTextField("--");
		doorState.setEditable(false);
		doorState.setPreferredSize(new Dimension(100,15));
		infoLine.add(new JLabel("Door State:"));
		infoLine.add(doorState);
		*/
		
		mainPanel.add(infoLine);
		mainPanel.add(Box.createRigidArea(new Dimension(0,20)));
		mainPanel.setPreferredSize(new Dimension(FIELD_WIDTH,FIELD_HEIGHT));

		// Button to simulate landing
		JPanel buttonPanel = new JPanel();
		landButton = new JButton("Land");
		landButton.setEnabled(false);
		landButton.addActionListener(this);

		// Button to simulate takeoff
		takeOffButton = new JButton("Take Off");
		takeOffButton.setEnabled(false);
		takeOffButton.addActionListener(this);

		// Button to simulate alarm
		alarmButton = new JButton("Start Alarm State");
		alarmButton.setEnabled(true);
		alarmButton.addActionListener(this);
		buttonPanel.add(alarmButton);

		/*
		reset = new JButton("Reset");
		reset.setEnabled(false);
		reset.addActionListener(this);
		*/

		buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.X_AXIS));	    
		buttonPanel.add(landButton);
		buttonPanel.add(takeOffButton);
		//buttonPanel.add(reset);
		
		mainPanel.add(buttonPanel);
		mainPanel.add(Box.createRigidArea(new Dimension(0,20)));

		    setLayout(new BorderLayout());

    //JPanel mainPanel = new JPanel();
    mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.Y_AXIS));

    // ... costruisci infoLine, buttonPanel, ecc ...
    mainPanel.add(infoLine);
    mainPanel.add(buttonPanel);

    // ✅ aggiungi davvero il mainPanel al JPanel
    add(mainPanel, BorderLayout.CENTER);


		//setContentPane(mainPanel);	
		/*
		addWindowListener(new WindowAdapter(){
			public void windowClosing(WindowEvent ev){
				System.exit(-1);
			}
		});











		*/
	}
	

	public void registerController(DashboardController contr){
		this.controller = contr;
	}


	/* SETTERS */

	public void setHangarState(String msg){
		SwingUtilities.invokeLater(() -> {
			hangarState.setText(msg); 
		});
	}

	public void setHangarTemperature(Float msg) {
		SwingUtilities.invokeLater(() -> {
			hangarTemperature.setText("" + msg);
		});
	}

	public void setGroundDistance(Float msg) {
		SwingUtilities.invokeLater(() -> {
			groundDistance.setText(""+msg);
		});
	}


	/* ENABLERS */

	public void enableTakeoff() {
		SwingUtilities.invokeLater(()-> {
			takeOffButton.setEnabled(true);
		});
	}

	public void enableLanding() {
		SwingUtilities.invokeLater(() -> {
			landButton.setEnabled(true);
		});
	}

	public void disableTakeoff() {
		SwingUtilities.invokeLater(() -> {
			takeOffButton.setEnabled(false);
		});
	}
	public void disableLanding() {
		SwingUtilities.invokeLater(() -> {
			landButton.setEnabled(false);
		});
	}

	// Cosa fare quando un tasto e' cliccato
	public void actionPerformed(ActionEvent ev){
		try {
			if (ev.getSource() == landButton) {			// Se premuto il tasto Land
				landButton.setEnabled(false);
				controller.notifyLanding();
			}
			else if (ev.getSource() == takeOffButton) { // se premuto il tasto takeoff
				takeOffButton.setEnabled(false);
				controller.notifyTakingOff();
			} else if (ev.getSource() == alarmButton) {
				alarmActive = !alarmActive;

				if (alarmActive) {
					alarmButton.setText("Stop Alarm State");
					controller.notifyStartAlarm(); 
				} else {
					alarmButton.setText("Start Alarm State");
					controller.notifyStopAlarm();
				}
			}
		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}
}
