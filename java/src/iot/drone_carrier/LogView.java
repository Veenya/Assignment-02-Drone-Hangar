package iot.drone_carrier;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.util.Date;

import javax.swing.*;
import javax.swing.border.Border;

class LogView extends JPanel  {

	private JTextArea log;

	public LogView(){
		super(new BorderLayout());
		log = new JTextArea(18, 16);
		

		//setLayout(new BorderLayout());

		log.setEditable(false);

		// Sfondo bianco + testo nero
		log.setBackground(Color.WHITE);
		log.setForeground(Color.BLACK);

		// Testo più grande (monospace comodo per log)
		log.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 18));

		// Scroll pane più grande
		add(new JScrollPane(log), BorderLayout.CENTER);

	}

	public void log(String msg){
		SwingUtilities.invokeLater(() -> {
			String date = new Date().toString();
			log.append("[" + date + "] " + msg + "\n");
			//log.setCaretPosition(log.getDocument().getLength()); // autoscroll in fondo
		});
	}

}
