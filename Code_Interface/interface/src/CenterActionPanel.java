import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;

import javax.swing.DefaultListModel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.ListSelectionModel;

import com.sun.xml.internal.ws.policy.privateutil.PolicyUtils.Collections;

public class CenterActionPanel extends JPanel{
	JList<String> actionList;
	DefaultListModel<String> actionModel;
	JScrollPane listScrollPane;
	File racine = new File("/home/pi/GrosRobot2019/fileaction/");
	
	public CenterActionPanel() {
		Main.cap = this;
		this.setBackground(Main.background_color);
		this.setForeground(Main.background_color);
		
		actionModel = new DefaultListModel<String>();
		actionList = new JList<String>(actionModel);
		actionList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		actionList.setBackground(new Color(60, 60, 60));
		actionList.setFont(new Font("Courier", Font.BOLD, 35));
		actionList.setForeground(Main.text_color);
		listScrollPane = new JScrollPane(actionList);
		listScrollPane.setPreferredSize(new Dimension(975, 450));

		listScrollPane.getHorizontalScrollBar().setPreferredSize(new Dimension(0,40));
		listScrollPane.getVerticalScrollBar().setPreferredSize(new Dimension(60,0));
		
		//File folder = new File("C:/Users/cecil/Documents/ENSIM_Elec/Code/PreCoupeNemaFusion/fileaction");
		File folder = new File("/home/pi/GrosRobot2019/fileaction");
		//TODO Changer le repertoire
		ArrayList<File> listFile = new ArrayList<File>();
		getAllFiles(folder, listFile);
		for(File file : listFile) {
			actionModel.addElement(racine.toURI().relativize(file.toURI()).getPath().replaceFirst("[.][^.]+$", ""));
		}
		this.add(listScrollPane);
	}
	
	public String getAction() {
		return actionList.getSelectedValue();
	}
	
	private void getAllFiles(File path, ArrayList<File> liste) {
		for(File file : path.listFiles()) {
			if(file.isDirectory()) {
				//repertoire
				getAllFiles(file, liste);
			}else {
				//fichier
				liste.add(file);
			}
		}
	}
	
}
