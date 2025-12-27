package iot.drone_carrier;

import javax.swing.SwingUtilities;

class DashboardLauncher   {

	static DashboardView view = null;
	static LogView log = null;

	static MainWindow mainWindow = null;

	public static void main(String[] args) throws Exception {	

		view = new DashboardView();
		log = new LogView();
		mainWindow = new MainWindow(view, log);

		String portName = "COM5"; //TODO: puoi metterlo come costante o in config
		DashboardController contr = new DashboardController(portName,view,log);
		view.registerController(contr);
		
		//view.display();
		//log.display();
		mainWindow.display();
	}
}