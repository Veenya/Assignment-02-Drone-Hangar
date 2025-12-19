// TODO: aggiungo il tasto di debug (o due: on, off) o simula allarme...
// TODO: simulare stringhe arduino per fare cose per finire interfaccia

package iot.drone_carrier;

import java.util.concurrent.TimeUnit;

public class MonitoringAgent extends Thread {

	SerialCommChannel channel;
	DashboardView view;
	LogView logger;

	private static final String STATE_PREFIX = "STATE,";
	private static final String ALARM_MSG = "ALARM,";
	private static final String LOG_PREFIX = "lo,";
	
	/* "ENUMS" */

	// Drone state Names
	static final String[] droneStates = {"Inside", "Operating", "Request Landing", "Request Takeoff"};  
	static final int INSIDE = 0;
	static final int OPERATING = 1;
	static final int REQUEST_LANDING = 2;
	static final int REQUEST_TAKEOFF = 3;

	// Hangar state Names
	static final String[] hangarStates = {"Normal", "Allarm", "Preallarm"};  
	static final int NORMAL = 0;
	static final int ALLARM = 1;
	static final int PREALLARM = 2;

	/*
	// Door state names
	static final String[] doorStates = {"Open", "Closed", "Moving", "Opening", "Closing"};  
	static final int OPEN = 0;
	static final int CLOSED = 1;
	static final int MOVING = 2;
	static final int OPENING = 3;
	static final int CLOSING = 4;
	*/

	/* ************************************************ */
	/* ******************* DEBUGGING ****************** */
	/* ************************************************ */
	/* How to make a message */
	/* State,<the drone state(Inside, Operating)>,<the hangar state (allarm, normal)>,<distance from ground, it's a number> */
	static final boolean DEBUGGING = true; 
	static final String debuggingMsg = "STATE,0,0,100";

	
	
	public MonitoringAgent(SerialCommChannel channel, DashboardView view, LogView log) throws Exception {
		this.view = view;
		this.logger = log;
		this.channel = channel;
	}
	
	public void run(){
		boolean isInside = true;

		boolean canLand = false;
		boolean canTakeoff = false;

		boolean inAllarm = false; 
		//boolean inPreallarm = false;
		//boolean doorOpen = false;


		/* Logica:
			se msg.startsWith("lo:") -> log
			se msg.startsWith("STATE,") -> split con ,
			se msg.startsWith("ALARM") -> gestisci allarme
		*/


		// -----------------------------------------------------------------------------
		
		while (true){
			try {

				/* DEBUGGING */
				String msg;
				if(DEBUGGING){
					msg = debuggingMsg;
				} else {
					msg = channel.receiveMsg();
				}

				logger.log("new msg: "+msg);

				TimeUnit.SECONDS.sleep(1);
				if (msg.startsWith(LOG_PREFIX)) {   // poi decide che messaggio e' (se inizia con log ava nella finestra di log, senno' aggiorna la gui)
					
					String cmd = msg.substring(LOG_PREFIX.length());
					System.out.println(cmd);

					// logger.log("new command: "+cmd);				
					// da DroneRemoteUnit::notifyNewState arduino dovrebbe mandare: "STATE:<dronestate>:<hangarState>:<distance>"
					// quindi il monitoringAgent ddeve riconoscere lo state, e fare parsing per gli altri campi
				} else if (msg.startsWith(STATE_PREFIX)){
					try {
						String args = msg.substring(STATE_PREFIX.length()); 
						String[] elems = args.split(",");
						
						if (elems.length >= 3) {
							// <drone>,<hangar>,<distance>
							int droneCode = Integer.parseInt(elems[0]);
							int hangarCode = Integer.parseInt(elems[1]);
							float groundDistance = Float.parseFloat(elems[2]);
							//String door = elems[4];
							//float temp = Float.parseFloat(elems[5]);
							
							
							view.setDroneState(droneStates[droneCode]);
							view.setHangarState(hangarStates[hangarCode]);
							//view.setDoorState(door);
							view.setGroundDistance(groundDistance);
							//view.setCurrentTemperature(temp);
							//view.setContainerState(stateNames[stateCode]);

							
							

							if (droneCode == INSIDE) {
								// disable landing
								view.disableLanding();
								// enable takeoff
								view.enableTakeoff();
							} else if (droneCode == OPERATING) {
								// disable takeoff
								view.disableTakeoff();
								// enable landing
								view.enableLanding();
							} else {
								view.enableLanding();
								view.enableTakeoff();
							}
						}
					} catch (Exception ex) {
						ex.printStackTrace();
						System.err.println("Error in msg: " + msg);
					}
				} else if (msg.startsWith(ALARM_MSG)){
					//this.logger.log(msg.substring(LOG_PREFIX.length()));
					String cmd = msg.substring(ALARM_MSG.length());
					System.out.println(cmd);
					logger.log("!!! ALARM !!! " + msg);
					view.setHangarState("ALARM");
					return;
				}
			} catch (Exception ex){
				ex.printStackTrace();
			}
		}

	}

}
