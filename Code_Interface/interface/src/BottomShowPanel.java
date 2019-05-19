import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class BottomShowPanel extends JPanel{
	
	JButton init_btn;
	JButton vider_btn;
	JButton run_btn;
	ProgrammeThread progThread;
	public BottomShowPanel() {
		this.setBackground(Main.background_color);
		this.setForeground(Main.background_color);
		
		vider_btn = new JButton("Ouvrir pinces");
		vider_btn.setForeground(new Color(45, 178, 255));
		vider_btn.setFont(new Font("Courier", Font.BOLD,Main.fontSize));
		
		init_btn = new JButton("Init nema");
		init_btn.setForeground(new Color(45, 178, 255));
		init_btn.setFont(new Font("Courier", Font.BOLD,Main.fontSize));
		
		run_btn = new JButton("  Lancer  ");
		run_btn.setForeground(new Color(45, 178, 255));
		run_btn.setFont(new Font("Courier", Font.BOLD,Main.fontSize));
		
		init_btn.addActionListener(init);
		run_btn.addActionListener(run);
		vider_btn.addActionListener(vider);
		
		this.add(init_btn);
		this.add(vider_btn);
		this.add(run_btn);
	}
	
	ActionListener init = new ActionListener(){
		   public void actionPerformed(ActionEvent e) {
				progThread = new ProgrammeThread("sudo /home/pi/GrosRobot2019/action init > /dev/null 2>&1");
				progThread.start();
		   }
	};
	
	ActionListener run = new ActionListener(){
		   public void actionPerformed(ActionEvent e) {
			   Main.frame.runFrame();
		   }
	};
	
	ActionListener vider = new ActionListener(){
		   public void actionPerformed(ActionEvent e) {
				progThread = new ProgrammeThread("sudo /home/pi/GrosRobot2019/action ouvrirPince");
				progThread.start();
		   }
	};
	
}
