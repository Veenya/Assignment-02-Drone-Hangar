/* IDEA:
> nuovo file MainWindow.java (JFrame) dove ci sara' tutto
> i vecchi file (LogView e DashboardView) diventano JPanel
*/

package iot.drone_carrier;

import javax.swing.*;
import java.awt.*;

public class MainWindow extends JFrame {

    public MainWindow(DashboardView dashboardView, LogView logView) {
        super("Drone Hangar - DRU");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(900, 400);

        JSplitPane split = new JSplitPane(
                JSplitPane.HORIZONTAL_SPLIT,
                dashboardView,
                logView
        );
        split.setResizeWeight(0.65); // 65% dashboard, 35% log

        setContentPane(split);
        setLocationRelativeTo(null);
    }

    public void display() {
        SwingUtilities.invokeLater(() -> setVisible(true));
    }
}
