import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;



public class BottomActionPanel extends JPanel{
	
	JButton lancer_btn;
	ProgrammeThread progThread;
	public BottomActionPanel() {
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
				progThread = new ProgrammeThread("sudo /home/pi/PreCoupeNemaFusion/action "+Main.cap.getAction()+" > /dev/null 2>&1");
				progThread.start();
		   }
	};
}
