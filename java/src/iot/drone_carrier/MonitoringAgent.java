// TODO: aggiungo il tasto di debug (o due: on, off) o simula allarme...
// TODO: simulare stringhe arduino per fare cose per finire interfaccia

package iot.drone_carrier;

import java.util.concurrent.TimeUnit;

public class MonitoringAgent extends Thread {

	SerialCommChannel channel;
	DashboardView view;
	LogView logger;

	private static final String STATE_PREFIX = "STATE,";
	private static final String ERROR_MSG = "ERROR,";
	private static final String LOG_PREFIX = "lo,";
	
	/* "ENUMS" */

	// Drone state Names
	public enum DroneState {
		INSIDE,
		OPERATING,
		REQUEST_LANDING,
		REQUEST_TAKEOFF;
	}
	public DroneState droneState = DroneState.INSIDE;

	

	// Hangar state Names
	public enum HangarState {
		NORMAL("Normal"),
		PREALARM("Prealarm"),
		ALARM("Alarm");

		private final String name;

		HangarState(String name) {
			this.name = name;
		}

		public String getName() {
			return name;
		}

		public static HangarState fromCode(int code) {
			if (code < 0 || code >= values().length) {
				throw new IllegalArgumentException("Invalid hangar code: " + code);
			}
			return values()[code];
		}
	}

	/* ************************************************ */
	/* ******************* DEBUGGING ****************** */
	/* ************************************************ */
	/* How to make a message */
	/* State,<the drone state(Inside, Operating)>,<the hangar state (allarm, normal)>,<distance from ground, it's a number> */
	// static final boolean DEBUGGING = true; 
	static final boolean DEBUGGING = false; 
	// static final String debuggingMsg = "STATE,1,0,100"; // STATE,<hangar>,<distance>,<temp>
	static final String debuggingMsg = "STATE,0,0,10,20.70"; // STATE,<hangar state>,<drone state><distance>,<temp>

	
	
	public MonitoringAgent(SerialCommChannel channel, DashboardView view, LogView log) throws Exception {
		this.view = view;
		this.logger = log;
		this.channel = channel;
	}
	
	public void run(){
		boolean canLand = false;
		boolean canTakeoff = false;


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
					//msg = "";
				} else {
					msg = channel.receiveMsg();
				}

				logger.log("new msg: "+msg);

				TimeUnit.MILLISECONDS.sleep(100);  // Pausa di 0.1 secondi
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
							// <hangar>,<distance>,<temp>
							int hangarCode = Integer.parseInt(elems[0]);
							float droneCode = Integer.parseInt(elems[1]);
							float groundDistance = Float.parseFloat(elems[2]);
							float hangarTemperature = Float.parseFloat(elems[3]);
							
							view.setHangarState(HangarState.fromCode(hangarCode).getName());
							view.setDroneState(droneCode);
							view.setGroundDistance(groundDistance);
							view.setHangarTemperature(hangarTemperature);
							// TODO Aggiungere stato drone
							
							if (droneCode == 0) {
								droneState = DroneState.INSIDE;
							} else if (droneCode == 1) {
								droneState = DroneState.OPERATING;
							}

							if (droneState == DroneState.INSIDE && !canTakeoff) { // inside
								canTakeoff = true;
								view.enableTakeoff();

							} else if (droneState == DroneState.OPERATING && !canLand) {
								canLand = true;
								view.enableLanding();
							} else if (droneState != DroneState.OPERATING && canLand) {
								canLand = false;
								view.disableLanding();
							}
							else if (droneState != DroneState.INSIDE && canTakeoff) { // inside
								canTakeoff = false;
								view.disableTakeoff();
							}

						}
					} catch (Exception ex) {
						ex.printStackTrace();
						System.err.println("Error in msg: " + msg);
					}
				} else if (msg.startsWith(ERROR_MSG)){
					//this.logger.log(msg.substring(LOG_PREFIX.length()));
					String cmd = msg.substring(ERROR_MSG.length());
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


	void setDroneStateInside() {
		this.droneState = DroneState.INSIDE;
	}

	void setDroneStateOperating() {
		this.droneState = DroneState.OPERATING;
	}
}
