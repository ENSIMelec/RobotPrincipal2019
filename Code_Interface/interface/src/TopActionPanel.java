import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
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
import javax.swing.border.Border;

import javafx.scene.layout.BorderImage;

public class TopActionPanel extends JPanel{

	JButton retour_btn;
	JLabel titre_lbl;

	public TopActionPanel() {
		this.setBackground(Main.background_color);
		this.setForeground(Main.background_color);
		this.setLayout(new BorderLayout());
		
		titre_lbl = new JLabel("Action");
		titre_lbl.setForeground(new Color(45, 178, 255));
		titre_lbl.setFont(new Font("Courier", Font.BOLD, Main.fontSize));
		titre_lbl.setHorizontalAlignment(SwingConstants.CENTER);
		
		retour_btn = new JButton("Retour");
		retour_btn.setForeground(new Color(45, 178, 255));
		retour_btn.setFont(new Font("Courier", Font.BOLD, Main.fontSize));
		retour_btn.setMargin(new Insets(10, 10, 10, 10));
		
		
		retour_btn.addActionListener(retour);
		
		this.add(retour_btn, BorderLayout.WEST);
		this.add(titre_lbl, BorderLayout.CENTER);
		this.add(Box.createRigidArea(new Dimension(200, 0)), BorderLayout.EAST);
	}
	
	ActionListener retour = new ActionListener(){
		   public void actionPerformed(ActionEvent e) {
			   Main.frame.homeFrame();
		   }
	};
	
}
