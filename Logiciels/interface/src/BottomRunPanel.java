import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.Box;
import javax.swing.JPanel;


/**
 *
 * CLASSE POUR LES COULEURS DE BOBBY : A SUPPRIMER
 *
 */

public class BottomRunPanel extends JPanel {

	Image couleur_img;
	public BottomRunPanel() {
		Main.brp = this;
		this.setBackground(Main.background_color);
		this.setForeground(Main.background_color);
		/*try {
			couleur_img = ImageIO.read(new File("/images/NON.png"));
		} catch (IOException e) {
			e.printStackTrace();
		}*/
		this.add(Box.createRigidArea(new Dimension(200, 200)), BorderLayout.EAST);
	}
	
	public void setCouleur(String str) {
		try {
			System.out.println("Image :/"+str+".png");
			couleur_img = ImageIO.read(getClass().getResource("/"+str+".png"));
			repaint();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	public void paintComponent(Graphics g){
		Graphics2D g2d = (Graphics2D)g;
		g.drawImage(couleur_img, 337, 0, this);
	}
}
