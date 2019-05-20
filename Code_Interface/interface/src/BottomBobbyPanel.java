import java.awt.Color;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JButton;
import javax.swing.JPanel;

/**
 *
 * CLASSE POUR LANCER KIROULPA
 *
 *
 */


public class BottomBobbyPanel extends JPanel{
	
	JButton lancer_btn;
	
	SSHThread sshThread;
	public BottomBobbyPanel() {
		this.setBackground(Main.background_color);
		this.setForeground(Main.background_color);
		
		lancer_btn = new JButton("Lancer");
		lancer_btn.setForeground(new Color(45, 178, 255));
		lancer_btn.setFont(new Font("Courier", Font.BOLD, Main.fontSize));

		lancer_btn.addActionListener(lancer);
		
		
		this.add(lancer_btn);
		
	}
	
	
	ActionListener lancer = new ActionListener(){
		   public void actionPerformed(ActionEvent e) {
			   if(sshThread!=null && sshThread.isAlive()) {
				   sshThread.stop();
			   }
			   sshThread = new SSHThread("sudo nohup /home/pi/PetitRobot2019/main "+Main.cbp.getStrat());
			   sshThread.start();
			   Main.frame.homeFrame();
		   }
	};
}
