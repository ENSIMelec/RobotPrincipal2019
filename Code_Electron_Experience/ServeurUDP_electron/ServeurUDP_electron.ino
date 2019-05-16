#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <String.h>

byte relON[] = {0xA0, 0x01, 0x01, 0xA2};  //Hex command to send to serial for open relay
byte relOFF[] = {0xA0, 0x01, 0x00, 0xA1}; //Hex command to send to serial for close relay

int status = WL_IDLE_STATUS;
const char* ssid = "MilhabotGaming";  //  your network SSID (name)
const char* pass = "Milharausecours!2018";       // your network password

unsigned int localPort = 5757;      // local port to listen for UDP packets

char packetBuffer[512]; //buffer to hold incoming and outgoing packets

char replyPacket[]; //buffer to answer the state of the request

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;

void setup()
{
  // Open serial communications and wait for port to open:
  //vitesse de communication avec la carte relais est de 9600
  Serial.begin(9600);

  WiFi.hostname("electron");
  // setting up Station AP
  WiFi.begin(ssid, pass);
 
  // Wait for connect to AP
  Serial.print("[Connecting]");
  Serial.print(ssid);
  int tries=0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    tries++;
    if (tries > 60){
      break;
    }
  }
  Serial.println();

  Serial.println("Connected to wifi");
  Serial.print("Udp server started at port ");
  Serial.println(localPort);

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  Udp.begin(localPort);
}

void loop() {

  int packetSize = Udp.parsePacket();
  if(packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i =0; i < 4; i++)
    {
      Serial.print(remote[i], DEC);
      if (i < 3)
      {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);

    int len = Udp.read(packetBuffer, 255);
    if (len > 0)
    {
      packetBuffer[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", packetBuffer);
    
    String message = String(packetBuffer);

    //Start electron
    if(!message.compareTo("On")) {
      Serial.println("Electron is ON");
      replyPacket = "OK"; 
      Serial.write(relON, sizeof(relON));   //serial frame which turn on the relay
    }
    //Stop experience
    else if(!message.compareTo("Connexion")) {
      Serial.println("Electron is OFF");
      replyPacket = "Reussie"; // a reply string to send back
      Serial.write(relOFF, sizeof(relOFF)); //serial frame which turn off the relay
    }
    else Serial.println("Error: not a matching command...");

    // send back a reply, to the IP address and port we got the packet from
    Serial.println("Contents:");
    Serial.println(packetBuffer);
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(replyPacket);
    Udp.endPacket();    
}
}
