import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import javax.imageio.ImageIO;
import javax.swing.Box;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingConstants;

public class CenterRunPanel extends JPanel {

	JLabel score_lbl;
	Component box;
	ProgrammeThread progThread;
	
	
	public CenterRunPanel() {
		Main.crp = this;
		this.setBackground(Main.background_color);
		this.setForeground(Main.background_color);
		this.setLayout(new BorderLayout());
		
		score_lbl = new JLabel("0 point");
		score_lbl.setForeground(new Color(45, 178, 255));
		score_lbl.setFont(new Font("Courier", Font.BOLD,140));
		score_lbl.setHorizontalAlignment(SwingConstants.CENTER);
		score_lbl.setVerticalAlignment(SwingConstants.CENTER);
		this.add(score_lbl, BorderLayout.CENTER);
		
		progThread = new ProgrammeThread("sudo /home/pi/PreCoupeNemaFusion/main "+Main.strat+" > /dev/null 2>&1");
		progThread.start();
		
	}
	
	public void stopProg() {
		progThread.stopProg();
	}
	
	public void setScore(int score) {
		score_lbl.setText(score+" points");
	}
}
