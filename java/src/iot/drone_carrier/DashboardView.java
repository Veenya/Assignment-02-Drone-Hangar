package iot.drone_carrier;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.*;

class DashboardView extends JPanel implements ActionListener  {

	// Send commands from GUI to Hangar simulating:
	private JButton landButton;      // - Landing
	private JButton takeOffButton;   // - Taking off
	private JButton alarmButton;     // Alarm simulation
	private boolean alarmActive = false;


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



		setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
		add(Box.createRigidArea(new Dimension(0, 10)));

		// Riga info
		JPanel infoLine = new JPanel();
        infoLine.setLayout(new BoxLayout(infoLine, BoxLayout.X_AXIS));


		

		//setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
		//this.setResizable(false);
		//JPanel mainPanel = new JPanel();
		//mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.Y_AXIS));
		//mainPanel.add(Box.createRigidArea(new Dimension(0,20)));
		


		// Hangar State
		hangarState = new JTextField("--");
		hangarState.setEditable(false);
		//hangarState.setPreferredSize(new Dimension(FIELD_WIDTH,FIELD_HEIGHT));
		hangarState.setMaximumSize(new Dimension(FIELD_WIDTH, FIELD_HEIGHT));
        infoLine.add(new JLabel("Hangar: "));
        infoLine.add(hangarState);
        infoLine.add(Box.createRigidArea(new Dimension(10, 0)));

		// Drone State
		hangarTemperature = new JTextField("--");
		hangarTemperature.setEditable(false);
		hangarTemperature.setMaximumSize(new Dimension(FIELD_WIDTH, FIELD_HEIGHT));
		infoLine.add(new JLabel("Drone State:")); 
		infoLine.add(hangarTemperature);
		infoLine.add(Box.createRigidArea(new Dimension(10, 0)));

		// Ground Distance
        groundDistance = new JTextField("--");
        groundDistance.setEditable(false);
        groundDistance.setMaximumSize(new Dimension(FIELD_WIDTH, FIELD_HEIGHT));
        infoLine.add(new JLabel("Distance: "));
        infoLine.add(groundDistance);
        infoLine.add(Box.createRigidArea(new Dimension(10, 0)));

		// Temperature of hangar
        currentTemperature = new JTextField("--");
        currentTemperature.setEditable(false);
        currentTemperature.setMaximumSize(new Dimension(FIELD_WIDTH, FIELD_HEIGHT));
        infoLine.add(new JLabel("Temp: "));
        infoLine.add(currentTemperature);

        add(infoLine);
        add(Box.createRigidArea(new Dimension(0, 12)));
		
		

		// Button to simulate landing
        // ---- Bottoni ----
        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.X_AXIS));

        landButton = new JButton("Land");
        landButton.setEnabled(false);
        landButton.addActionListener(this);

        takeOffButton = new JButton("Take Off");
        takeOffButton.setEnabled(false);
        takeOffButton.addActionListener(this);

        alarmButton = new JButton("Start Alarm State");
        alarmButton.setEnabled(true);
        alarmButton.addActionListener(this);

        buttonPanel.add(landButton);
        buttonPanel.add(Box.createRigidArea(new Dimension(10, 0)));
        buttonPanel.add(takeOffButton);
        buttonPanel.add(Box.createRigidArea(new Dimension(10, 0)));
        buttonPanel.add(alarmButton);

        add(buttonPanel);
        add(Box.createRigidArea(new Dimension(0, 10)));
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
