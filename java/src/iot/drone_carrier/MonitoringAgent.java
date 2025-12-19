// TODO: aggiungo il tasto di debug (o due: on, off) o simula allarme...
// TODO: simulare stringhe arduino per fare cose per finire interfaccia

/* Ogni riga e' uno di questi tipi:
    log -> lo:<testo libero>
	stato-> STATE,<drone>,<hangar>,<door>,<distance>,<temp>
	allarme -> ALARM (opzionale: ALARM,<reason>)
	(opzionale, prealarm)
 */
package iot.drone_carrier;

public class MonitoringAgent extends Thread {

	SerialCommChannel channel;
	DashboardView view;
	LogView logger;

	private static final String STATE_PREFIX = "STATE,";
	private static final String ALARM_MSG = "ALARM,";
	private static final String LOG_PREFIX = "lo,";
	
	/* "ENUMS" */
	// State Names
	static final String[] stateNames = {"Normal", "Allarm", "Preallarm"};  
	static final int NORMAL = 0;
	static final int ALLARM = 1;
	static final int PREALLARM = 2;

	// Door state names
	static final String[] doorStateNames = {"Open", "Closed", "Moving", "Opening", "Closing"};  
	static final int OPEN = 0;
	static final int CLOSED = 1;
	static final int MOVING = 2;
	static final int OPENING = 3;
	static final int CLOSING = 4;

	
	
	public MonitoringAgent(SerialCommChannel channel, DashboardView view, LogView log) throws Exception {
		this.view = view;
		this.logger = log;
		this.channel = channel;
	}
	
	public void run(){
		boolean canLand = false;
		boolean canTakeoff = false;

		//boolean inAllarm = false; 
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
				String msg = channel.receiveMsg();  // bloccante, aspetta una riga dal seriale finche' arduino non manda \n
				logger.log("new msg: "+msg);				
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
							String drone = elems[0];
							String hangar = elems[1];
							String dist = elems[2];
							//String door = elems[4];
							//float temp = Float.parseFloat(elems[5]);
							
							
							view.setDroneState(drone);
							view.setHangarState(hangar);
							//view.setDoorState(door);
							view.setGroundDistance(dist);
							//view.setCurrentTemperature(temp);
						//view.setContainerState(stateNames[stateCode]);

							
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
