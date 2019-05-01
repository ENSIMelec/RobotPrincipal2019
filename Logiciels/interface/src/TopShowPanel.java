import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Font;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;

import javax.swing.Box;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingConstants;

public class TopShowPanel extends JPanel {

	JButton retour_btn;
	JLabel strat_lbl;
	
	Component box;
	
	public TopShowPanel() {
		this.setBackground(Main.background_color);
		this.setForeground(Main.background_color);
		this.setLayout(new BorderLayout());
		
		retour_btn = new JButton("Retour");
		retour_btn.setForeground(new Color(45, 178, 255));
		retour_btn.setFont(new Font("Courier", Font.BOLD, Main.fontSize));
		retour_btn.setHorizontalAlignment(SwingConstants.LEFT);
		retour_btn.setMargin(new Insets(10, 10, 10, 10));
		
		strat_lbl = new JLabel(Main.strat);
		strat_lbl.setForeground(new Color(45, 178, 255));
		strat_lbl.setFont(new Font("Courier", Font.BOLD, Main.fontSize));
		strat_lbl.setHorizontalAlignment(SwingConstants.CENTER);
		
		retour_btn.addActionListener(retour);
		
		box = Box.createHorizontalStrut(150);
		
		this.add(retour_btn,BorderLayout.WEST);
		this.add(strat_lbl, BorderLayout.CENTER);
		this.add(box, BorderLayout.EAST);
	}
	
	ActionListener retour = new ActionListener(){
		   public void actionPerformed(ActionEvent e) {
			   Main.frame.selectionFrame();
		   }
	};
}
