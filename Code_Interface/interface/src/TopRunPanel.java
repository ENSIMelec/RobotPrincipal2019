import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.text.DecimalFormat;

import javax.swing.Box;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingConstants;

public class TopRunPanel extends JPanel{
	
	JLabel temps_lbl;
	JButton stop_btn;
	public TopRunPanel() {
		Main.trp = this;
		this.setBackground(Main.background_color);
		this.setForeground(Main.background_color);
		this.setLayout(new BorderLayout());
		
		temps_lbl = new JLabel("100");
		temps_lbl.setForeground(new Color(45, 178, 255));
		temps_lbl.setFont(new Font("Courier", Font.BOLD,45));
		temps_lbl.setHorizontalAlignment(SwingConstants.CENTER);
		
		stop_btn = new JButton("STOP");
		stop_btn.setForeground(new Color(45, 178, 255));
		stop_btn.setFont(new Font("Courier", Font.BOLD,Main.fontSize));
		stop_btn.setHorizontalAlignment(SwingConstants.RIGHT);
		stop_btn.setMargin(new Insets(10, 10, 10, 10));
		
		stop_btn.addActionListener(stop);
		
		this.add(Box.createRigidArea(new Dimension(175, 0)), BorderLayout.WEST);
		this.add(temps_lbl, BorderLayout.CENTER);
		this.add(stop_btn, BorderLayout.EAST);
	}
	
	public void setTemps(Float tps) {
		if(100-tps*0.001>0) {
			temps_lbl.setText(String.valueOf(new DecimalFormat("0.00").format(100-tps*0.001)));
		}else {
			temps_lbl.setText("0.00");
		}
	}
	
	ActionListener stop = new ActionListener(){
		   public void actionPerformed(ActionEvent e) {
			   Main.crp.stopProg();
			   Main.frame.showFrame();
		   }
	};
}
