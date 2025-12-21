package iot.drone_carrier;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.util.Date;

import javax.swing.*;

class LogView extends JFrame  {

	private JTextArea log;

	public LogView(){
		super("Log");

		// Finestra più grande
		setSize(900, 700);
		this.setResizable(false);

		JPanel mainPanel = new JPanel();

		// Area di log più grande
		log = new JTextArea(35, 80);

		// Importante: NON disabilitarla (setEnabled(false)) altrimenti diventa grigia.
		// Basta renderla non editabile.
		log.setEditable(false);

		// Sfondo bianco + testo nero
		log.setBackground(Color.WHITE);
		log.setForeground(Color.BLACK);

		// Testo più grande (monospace comodo per log)
		log.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 18));

		// Scroll pane più grande
		JScrollPane scroll = new JScrollPane(log);
		scroll.setPreferredSize(new Dimension(860, 650));

		mainPanel.add(scroll);
		this.getContentPane().add(mainPanel);
	}

	public void log(String msg){
		SwingUtilities.invokeLater(() -> {
			String date = new Date().toString();
			log.append("[" + date + "] " + msg + "\n");
			log.setCaretPosition(log.getDocument().getLength()); // autoscroll in fondo
		});
	}

	public void display() {
		SwingUtilities.invokeLater(() -> {
			this.setVisible(true);
		});
	}
}
