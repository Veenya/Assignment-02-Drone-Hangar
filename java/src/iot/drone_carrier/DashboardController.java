package iot.drone_carrier;
/*
The DRU subsystem is meant to have a GUI with controls to:

send command to the hangar, simulating behaviour of the drone (taking off and landing).
visualise:
the current state of the drone (rest, taking off, operating, landing);
the current state of the drone hangar (normal, alarm);
(when landing) the current distance to ground.
*/


public class DashboardController  {

	static final String MSG_TAKING_OFF  = "to";
	static final String MSG_LANDING 	= "la";
	
	static final String MSG_RESET       = "re";
	static final String MSG_OPERATING 	= "op";
	
	SerialCommChannel channel;
	DashboardView view;
	LogView logger;
	
	public DashboardController(String port, DashboardView view, LogView logger) throws Exception {
		this.view = view;
		this.logger = logger;
		
		channel = new SerialCommChannel(port,9600);		
		new MonitoringAgent(channel,view,logger).start();
			
		/* attesa necessaria per fare in modo che Arduino completi il reboot */
		
		System.out.println("Waiting Arduino for rebooting...");		
		Thread.sleep(4000);
		System.out.println("Ready.");		
	
	}


	public void notifyReset() {
		channel.sendMsg(MSG_RESET);
	}
	
	public void notifyTakingOff() {
		channel.sendMsg(MSG_TAKING_OFF);
		logger.log("TAKING OFF");
	}

	public void notifyOperating() {
		channel.sendMsg(MSG_OPERATING);
		logger.log("OPERATING");
	}
	
	public void notifyLanding() {
		channel.sendMsg(MSG_LANDING);
		logger.log("LANDING");
	}

}
