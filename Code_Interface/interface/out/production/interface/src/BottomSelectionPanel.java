

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class BottomSelectionPanel extends JPanel{
	JButton selection_btn;
	public BottomSelectionPanel() {
		this.setBackground(Main.background_color);
		this.setForeground(Main.background_color);

		
		selection_btn = new JButton("Selectionner");
		selection_btn.setForeground(new Color(45, 178, 255));
		selection_btn.setFont(new Font("Courier", Font.BOLD, Main.fontSize));
		
		
		selection_btn.addActionListener(selection);
		
		this.add(selection_btn);
	}
	
	ActionListener selection = new ActionListener(){
		   public void actionPerformed(ActionEvent e) {
			   Main.strat = Main.csp.getStrat();
			   Main.frame.showFrame();
		   }
	};

}
