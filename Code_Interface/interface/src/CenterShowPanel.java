import java.awt.Component;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.JPanel;

public class CenterShowPanel extends JPanel {

	//Image table_img;
	
	public CenterShowPanel() {
		this.setBackground(Main.background_color);
		this.setForeground(Main.background_color);
		try {
			//File folder = new File("/home/pi/GrosRobot2019/fileactionpoint3.0/"+Main.strat+"/table.png");
			//File file = new File("C:/Users/cecil/Documents/ENSIM_Elec/Code/PreCoupeNemaFusion/filepoint3.0/"+Main.strat+"/table.png");
			//TODO changer le chemin
			System.out.println(Main.strat);
			/*if(file.exists()) {
				table_img = ImageIO.read(file);
			}else {
				table_img = ImageIO.read(new File("/images/table.png"));
			}*/
			
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public void paintComponent(Graphics g){
		Graphics2D g2d = (Graphics2D)g;
		//g.drawImage(table_img, 60, 0, this);
	}
}
