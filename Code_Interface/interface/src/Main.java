import java.awt.Color;
import java.awt.GraphicsDevice;
import java.awt.GraphicsEnvironment;

import javax.swing.JFrame;

public class Main {
	public final static int port = 1111;
	public static CenterSelectionPanel csp;
	public static CenterActionPanel cap;
	public static CenterBobbyPanel cbp;
	public static TopRunPanel trp;
	public static CenterRunPanel crp;
	public static BottomRunPanel brp;
	public static Frame frame;
	public static String strat;
	public static Color background_color = new Color(42, 42, 42);
	public static Color text_color = new Color(223, 223, 223);
	public static Color title_color = new Color(45, 178, 255);
	public static int fontSize = 45;
	
	public static void main(String[] args) {
		frame = new Frame();
		frame.getContentPane().setBackground(background_color);
		GraphicsDevice d = GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice();
		d.setFullScreenWindow(frame); //TODO sup commentaire
		
		ComThread comThread = new ComThread(port);
		comThread.start();
	}
}
