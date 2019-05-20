import com.jcabi.ssh.Shell;
import com.jcabi.ssh.SshByPassword;

import java.io.IOException;
import java.net.UnknownHostException;

public class SSHThread extends Thread{
	
	String cmd;
	Shell shell;
	public SSHThread(String str) {
		cmd = str;
		try {
			/**
			 * Adresse de Milhabot ou Kiroulpa à changer (ils ont la même...
			 */

			shell = new SshByPassword("172.30.1.1", 22, "pi", "Milharausecours!2018");
		} catch (UnknownHostException e) {
			e.printStackTrace();
		}
	}
	
	public void run() {
		try {
			String stdout = new Shell.Plain(shell).exec(cmd);
			System.out.println(stdout);
		} catch (IOException e1) {
			e1.printStackTrace();
		}
	}
}
