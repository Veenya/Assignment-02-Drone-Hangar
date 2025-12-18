package iot.drone_carrier;

public class MonitoringAgent extends Thread {

	SerialCommChannel channel;
	DashboardView view;
	LogView logger;
	

	// --------------------------------------------------------------

	/* La parte che ho separato coi --- potrebbe essere sostituita da:
	   private static final String STATE_PREFIX = "STATE:";
	   private static final String ALARM_MSG = "ALARM:";
	   private static final String LOG_PREFIX = "lo:"

	*/
	static final String CARRIER_PREFIX 	=  "cw:";
	static final String LOG_PREFIX 	=  "lo:";
	static final String MSG_STATE 		= "st:";
	
	/*
	the current state of the drone (rest, taking off, operating, landing);
	the current state of the drone hangar (normal, alarm); (si sa dalla temperatura)
	stato della porta (aperta, inApertura, inChiusura, chiusa)
	*/

	static final String[] stateNames = {"Normal", "Allarm", "Preallarm"};  
	static final int NORMAL = 0;
	static final int ALLARM = 1;
	static final int PREALLARM = 2;
	static final String[] doorStateNames = {"Open", "Close", "Opening", "Closing"};  
	static final int OPEN = 0;

	// --------------------------------------------------------------
	
	
	public MonitoringAgent(SerialCommChannel channel, DashboardView view, LogView log) throws Exception {
		this.view = view;
		this.logger = log;
		this.channel = channel;
	}
	
	public void run(){
		boolean inAllarm = false; 
		boolean inPreallarm = false;
		boolean doorOpen = false;
		boolean canLand = false;
		boolean canTakeoff = false;

		while (true){
			try {
				String msg = channel.receiveMsg();  // bloccante, aspetta una riga dal seriale finche' arduino non manda \n
				//if (msg == null) continue; //todo cambia
				logger.log("new msg: "+msg);				
				if (msg.startsWith(CARRIER_PREFIX)){   // poi decide che messaggio e' (se inizia con log ava nella finestra di log, senno' aggiorna la gui)
					String cmd = msg.substring(CARRIER_PREFIX.length()); 
					// logger.log("new command: "+cmd);				
					// da DroneRemoteUnit::notifyNewState arduino dovrebbe mandare: "STATE:<dronestate>:<hangarState>:<distance>"
					// quindi il monitoringAgent ddeve riconoscere lo state, e fare parsing per gli altri campi
					if (cmd.startsWith(MSG_STATE)){
						try {
							String args = cmd.substring(MSG_STATE.length()); 
							
							// TODO: cambiare tutta la logica
							/*
							the current state of the drone (rest, taking off, operating, landing);
							the current state of the drone hangar (normal, alarm);
							(when landing) the current distance to ground.
							*/
							String[] elems = args.split(":"); // Nota: usare lo stesso formalismo del prof: statecode:wastelevel:temp  -> statecode:temp
							if (elems.length >= 3) {
								int stateCode = Integer.parseInt(elems[0]);        // stato drone
								int doorStateCode = Integer.parseInt(elems[0]); //todo: non so se e' giusto
								float groundDistance = Float.parseFloat(elems[1]); // distanza dall'hangar (da terra) del drone 
								float temp = Float.parseFloat(elems[2]);           // temperatura hangar
		
								view.setGroundDistance(groundDistance);
								view.setCurrentTemperature(temp);
								view.setContainerState(stateNames[stateCode]);
								
									/*
									static final String MSG_REST 	    = "re";
									static final String MSG_TAKING_OFF  = "to";
									static final String MSG_OPERATING 	= "op";
									static final String MSG_LANDING 	= "la";

									view.enableLanding();
									view.enableTakeoff();

									canTakeoff = false;
									canLand = false;
									*/
								// TODO: rivedere e aggiungere quello che manca
	
								
								if (stateCode == NORMAL && doorStateCode == OPEN && !canTakeoff) { // maintenance
									canTakeoff = true;
									view.enableTakeoff();
								} else if (stateCode == NORMAL && doorStateCode == OPEN && !canLand) { // maintenance
									canLand = true;
									view.enableLanding();
								} else if ((stateCode != NORMAL || doorStateCode != OPEN) && canTakeoff) {
									canTakeoff = false;
									view.disableTakeoff();
								} else if ((stateCode != NORMAL || doorStateCode != OPEN) && canLand) {
									canLand = false;
									view.disableLanding();
								}
								
								
							}
						} catch (Exception ex) {
							ex.printStackTrace();
							System.err.println("Error in msg: " + cmd);
						}
					}
				} else if (msg.startsWith(LOG_PREFIX)){
					this.logger.log(msg.substring(LOG_PREFIX.length()));
				}
			} catch (Exception ex){
				ex.printStackTrace();
			}
		}
	}

}
