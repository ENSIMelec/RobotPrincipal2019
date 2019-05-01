import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Font;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingConstants;

public class TopHomePanel extends JPanel{
	JButton quitter_btn;
	JLabel titre_lbl;
	JButton eteindre_btn;

	public TopHomePanel() {
		this.setBackground(Main.background_color);
		this.setForeground(Main.background_color);
		this.setLayout(new BorderLayout());
		
		titre_lbl = new JLabel("Menu Principal");
		titre_lbl.setForeground(new Color(45, 178, 255));
		titre_lbl.setFont(new Font("Courier", Font.BOLD, Main.fontSize));
		titre_lbl.setHorizontalAlignment(SwingConstants.CENTER);
		
		quitter_btn = new JButton("Quitter");
		quitter_btn.setForeground(new Color(45, 178, 255));
		quitter_btn.setFont(new Font("Courier", Font.BOLD, Main.fontSize));
		quitter_btn.setMargin(new Insets(10, 10, 10, 10));
		
		eteindre_btn = new JButton("Eteindre");
		eteindre_btn.setForeground(new Color(45, 178, 255));
		eteindre_btn.setFont(new Font("Courier", Font.BOLD, Main.fontSize));
		eteindre_btn.setMargin(new Insets(10, 10, 10, 10));
		
		quitter_btn.addActionListener(quitter);
		eteindre_btn.addActionListener(eteindre);
		
		this.add(quitter_btn, BorderLayout.WEST);
		this.add(titre_lbl, BorderLayout.CENTER);
		this.add(eteindre_btn, BorderLayout.EAST);
	}
	
	ActionListener quitter = new ActionListener(){
		   public void actionPerformed(ActionEvent e) {
			   System.exit(1);
		   }
	};
	
	ActionListener eteindre = new ActionListener(){
		   public void actionPerformed(ActionEvent e) {
			   try {
				java.lang.Runtime.getRuntime().exec("sudo shutdown now");
			   } catch (IOException e1) {
				   e1.printStackTrace();
			   }
		   }
	};
}
