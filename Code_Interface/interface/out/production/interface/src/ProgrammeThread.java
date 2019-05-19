import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class ProgrammeThread extends Thread{

	Process prog;
	String cmd;
	public ProgrammeThread(String str) {
		cmd = str;
	}
	
	public void run() {
		//String command = "/bin/bash -c 'sudo /home/pi/RobotPrincipal2018/main "+Main.strat+"'";
		try {
			System.out.println(cmd);
			prog = Runtime.getRuntime().exec(cmd);
		    BufferedReader reader = new BufferedReader(new InputStreamReader(prog.getInputStream()));
		    String line = "";
		    while((line = reader.readLine()) != null) {
		           System.out.print(line + "\n");
		    }
		    prog.waitFor();
		} catch (IOException | InterruptedException e) {
			e.printStackTrace();
		}
	}
	
	public void stopProg() {
		if(prog != null) {
			System.out.println("Arret du programme");
			prog.destroy();
		}else {
			System.out.println("Programme pas lancé");
		}
	}
}
