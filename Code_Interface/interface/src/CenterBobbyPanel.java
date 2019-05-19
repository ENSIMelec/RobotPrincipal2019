import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.Arrays;

import javax.swing.DefaultListModel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.ListSelectionModel;

/**
 *
 *
 * CLASSE POUR BOBBY : A SUPPRIMER
 *
 *
 */


public class CenterBobbyPanel extends JPanel{
	JList<String> stratList;
	DefaultListModel<String> stratModel;
	JScrollPane listScrollPane;
	JScrollBar bar;
	
	public CenterBobbyPanel() {
		Main.cbp = this;
		this.setBackground(Main.background_color);
		this.setForeground(Main.background_color);
		
		stratModel = new DefaultListModel<String>();
		stratList = new JList<String>(stratModel);
		stratList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		stratList.setBackground(new Color(60, 60, 60));
		stratList.setFont(new Font("Courier", Font.BOLD, 35));
		stratList.setForeground(Main.text_color);
		listScrollPane = new JScrollPane(stratList);
		listScrollPane.setPreferredSize(new Dimension(800, 450));
		bar = listScrollPane.getVerticalScrollBar();
		bar.setPreferredSize(new Dimension(60, 0));
		
		BufferedReader br=null;
		try {
			//br = new BufferedReader(new FileReader("D:/Users/Cayon/Documents/EnsimElec/Robot Cities/RobotPrincipal2018/stratBobby.txt"));
			br = new BufferedReader(new FileReader("/home/pi/GrosRobot2019/stratKiroulpa.txt"));
			//TODO Changer le repertoire
		} catch (FileNotFoundException e1) {
			e1.printStackTrace();
		}
		
		String line = null;
		do {
			try {
				line = br.readLine();
				stratModel.addElement(line);
			} catch (IOException e) {
				e.printStackTrace();
			}
			
		}while(line !=null);
		
		this.add(listScrollPane);
	}

	
	public String getStrat() {
		return stratList.getSelectedValue();
	}
}
