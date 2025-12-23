package iot.drone_carrier;

class DashboardLauncher   {
	static DashboardView view = null;
	static LogView log = null;

	public static void main(String[] args) throws Exception {	
		view = new DashboardView();
		log = new LogView();
		
		String portName = "COM5"; // puoi metterlo come costante o in config
		DashboardController contr = new DashboardController(portName,view,log);
		view.registerController(contr);
		
		view.display();
		log.display();
	}
}