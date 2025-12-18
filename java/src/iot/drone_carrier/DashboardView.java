/*
The DRU subsystem is meant to have a GUI with controls to:

send command to the hangar, simulating behaviour of the drone (taking off and landing).
visualise:
the current state of the drone (rest, taking off, operating, landing);
the current state of the drone hangar (normal, alarm);
(when landing) the current distance to ground.
*/

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

class DashboardView extends JFrame implements ActionListener  {

	private JButton maintenanceDone;
	private JButton dischargeContainer;

	private JButton land;
	//private JButton openDoor; //todo: rivedere il meccanismo di come funziona
	private JButton takeOff;
	private JButton reset;
	
	private JTextField doorState;           // stato della porta (aperta, chiusa, movimento)
	private JTextField currentTemperature; // temp hangar
	
	private JTextField containerState;      // stato hangar (normale, allarme e preallarme)
	private DashboardController controller;	
	
	public DashboardView(){
		super(".:: Drone Carrier ::.");
		setSize(600,150);
		this.setResizable(false);
		JPanel mainPanel = new JPanel();
		mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.Y_AXIS));
		mainPanel.add(Box.createRigidArea(new Dimension(0,20)));
		
		JPanel infoLine = new JPanel();
		infoLine.setLayout(new BoxLayout(infoLine, BoxLayout.LINE_AXIS));
		containerState = new JTextField("--");
		containerState.setEditable(false);
		containerState.setPreferredSize(new Dimension(200,15));
		infoLine.add(new JLabel("Hangar State:")); 
		infoLine.add(containerState);

		doorState = new JTextField("--");
		doorState.setEditable(false);
		doorState.setPreferredSize(new Dimension(100,15));
		infoLine.add(new JLabel("Door State:"));
		infoLine.add(doorState);

		currentTemperature = new JTextField("--");
		currentTemperature.setEditable(false);
		currentTemperature.setPreferredSize(new Dimension(200,15));
		infoLine.add(new JLabel("Current Temperature:"));
		infoLine.add(currentTemperature);
		
		mainPanel.add(infoLine);
		mainPanel.add(Box.createRigidArea(new Dimension(0,20)));
		mainPanel.setPreferredSize(new Dimension(200,20));

		JPanel buttonPanel = new JPanel();
		land = new JButton("Land");
		land.setEnabled(false);
		land.addActionListener(this);

		takeOff = new JButton("Take Off");
		takeOff.setEnabled(false);
		takeOff.addActionListener(this);

		reset = new JButton("Reset");
		reset.setEnabled(false);
		reset.addActionListener(this);

		buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.X_AXIS));	    
		buttonPanel.add(land);
		buttonPanel.add(takeOff);
		buttonPanel.add(reset);
		
		mainPanel.add(buttonPanel);
		mainPanel.add(Box.createRigidArea(new Dimension(0,20)));
		setContentPane(mainPanel);	
		
		addWindowListener(new WindowAdapter(){
			public void windowClosing(WindowEvent ev){
				System.exit(-1);
			}
		});
	}
	
	public void display() {
		SwingUtilities.invokeLater(() -> {
			this.setVisible(true);
		});
	}

	public void registerController(DashboardController contr){
		this.controller = contr;
	}

	public void setContainerState(String msg){
		SwingUtilities.invokeLater(() -> {
			containerState.setText(msg); 
		});
	}

	public void setGroundDistance(float perc){
		SwingUtilities.invokeLater(() -> {
			doorState.setText("" + perc);
		});
	}

	public void setCurrentTemperature(float temp){
		SwingUtilities.invokeLater(() -> {
			currentTemperature.setText("" + temp);
		});
	}
/* 
	public void enableAvailable() {
		SwingUtilities.invokeLater(() -> {
			maintenanceDone.setEnabled(false);
			dischargeContainer.setEnabled(false);
		});
	}
	
	public void enableMaintenance() {
		SwingUtilities.invokeLater(() -> {
			maintenanceDone.setEnabled(true);
			dischargeContainer.setEnabled(false);
		});
	}

	public void enableDischarge() {
		SwingUtilities.invokeLater(() -> {
			maintenanceDone.setEnabled(false);
			dischargeContainer.setEnabled(true);
		});
	}*/

	// todo togli
	public void enableAvailable() {}
	public void enableMaintenance() {}
	public void enableDischarge() {}

	// todo: fare
	public void enableTakeoff() {
		SwingUtilities.invokeLater(()-> {
			land.setEnabled(true);
			//porta si apre
			takeOff.setEnabled(false);
		});
	}

	public void enableLanding() {
		SwingUtilities.invokeLater(() -> {
			land.setEnabled(false);
			// porta si apre
			takeOff.setEnabled(true);
		});
	}

	public void disableTakeoff() {
		SwingUtilities.invokeLater(() -> {
			takeOff.setEnabled(false);
		});
	}
	public void disableLanding() {
		SwingUtilities.invokeLater(() -> {
			land.setEnabled(false);
		});
	}

	
	public void actionPerformed(ActionEvent ev){
		  try {
			  if (ev.getSource() == land){
				  controller.notifyLanding();
				  land.setEnabled(false);
			  } else if (ev.getSource() == takeOff){
				  controller.notifyTakingOff();
				  takeOff.setEnabled(false);
			  } else if (ev.getSource() == reset) {
				  controller.notifyReset();
				  reset.setEnabled(false);
			  }
		  } catch (Exception ex){
			  ex.printStackTrace();

		  }
	}
}
