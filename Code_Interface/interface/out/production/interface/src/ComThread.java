import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;

public class ComThread  extends Thread {
	
	private DatagramSocket serveur;

	public ComThread(int port) {
		try {
			serveur = new DatagramSocket(port);
		} catch (SocketException e) {
			e.printStackTrace();
		}
		System.out.println("Serveur ok attente de client");
	}
	
	public void run() {
		byte[] buffer = new byte[1024];
		DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
		while (true) {
            //System.out.println("Attente de message client");        
            try {
				serveur.receive(packet);
			} catch (IOException e) {
				e.printStackTrace();
			}
            

            String str = new String(packet.getData(),0,packet.getLength()-1);
	            switch(str.charAt(0)) {
	        	case 'V': tempsTraitement(str); break;		//Vitesse [V temps vitG cmdG vitD cmdD]
	        	case 'M': tempsTraitement(str); break;		//Commande moteur [M temps cmdG cmdD]
	        	case 'A': tempsTraitement(str); break;		//Angle	[A temps angle angleCible]
	        	case 'P': pointsTraitement(str); break;		//Point [P nbPoint]
	        	case 'U': couleurTraitement(str); break;	//CoUleur [U Initiale3Couleurs]
            }

  
            //On reinitialise la taille du datagramme, pour les futures receptions
            packet.setLength(buffer.length);                  
		}
	}
	
	private void pointsTraitement(String str) {
		String[] data = str.split(" ");
		//System.out.println("Point :"+data[1]);
		if(Main.crp!=null) {
			Main.crp.setScore(Integer.valueOf(data[1]));
		}
	}
	
	private void tempsTraitement(String str) {
		String[] data = str.split(" ");
		//System.out.println("Point :"+data[1]);
		if(Main.trp!=null) {
			Main.trp.setTemps(Float.valueOf(data[1]));
		}
	}
	
	private void couleurTraitement(String str) {
		String[] data = str.split(" ");
		if(Main.brp!=null) {
			Main.brp.setCouleur(data[1]);
		}
	}
	
}
