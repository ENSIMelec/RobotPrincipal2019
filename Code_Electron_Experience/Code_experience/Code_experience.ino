#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <Adafruit_NeoPixel.h>
#include <String.h>

int status = WL_IDLE_STATUS;
const char* ssid = "MilhabotGaming";  //  your network SSID (name)

const char* pass = "****";       // your network password

unsigned int localPort = 5757;      // local port to listen for UDP packets

char packetBuffer[512]; //buffer to hold incoming and outgoing packets
//string replyPacketConnexion[8];   //buffer to answer the state of the connection request 
//string replyPacketCommand[3];     //buffer to answer the state of the request

char* replyPacket;

bool doAnimations = false;
//LED strip declaration
const int NB_LED = 9;
const int PIN_LED = D3;
Adafruit_NeoPixel ledStrip = Adafruit_NeoPixel(NB_LED, PIN_LED); //number of LEDs, pin

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;

void setup()
{
  // led strip initialisation
  pinMode(PIN_LED, OUTPUT);
  ledStrip.begin();
  ledStrip.setBrightness(40); //0 to 255
  clearStrip(); //led strip off
  
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  WiFi.hostname("experience");
  // setting up Station AP
  WiFi.begin(ssid, pass);
 
  // Wait for connect to AP
  Serial.print("[Connecting]");
  Serial.print(ssid);
  int tries=0;
  ledStrip.setPixelColor((uint16_t)0, 0xFF0000); //led lights red while the experience is not connectec to the robot
  ledStrip.show();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    tries++;
    if (tries > 60){
      break;
    }
  }
  Serial.println();
  clearStrip();

  Serial.println("Connected to wifi");
  Serial.print("Udp server started at port ");
  Serial.println(localPort);
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

    
    //Start experience
    if(!message.compareTo("On")) {
      Serial.println("Experience is ON");
      replyPacket = "Ok"; 
      doAnimations = true;
    }
    //Connexion experience
    else if(!message.compareTo("Connexion")) {
          Serial.println("Experience is CONNECTED");
          replyPacket = "Reussie"; // a reply string to send back
    }
    else Serial.println("Error: not a matching command...");

    Serial.flush();
    
    // send back a reply, to the IP address and port we got the packet from
    Serial.println("Contents:");
    Serial.println(packetBuffer);
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(replyPacket);
    Udp.endPacket();

    memset(packetBuffer, 0, sizeof(packetBuffer));
    memset(replyPacket, 0, sizeof(replyPacket));

    if(doAnimations){
      //animations
      for(int i=0; i<5; i++) {
        alternateColor(0xFF0000, 0x0000FF, 500);
        delay(250);
        randomColorFill(500);
        delay(250);
        middleFill(0xFF000, 500);
        delay(250);
        randomPositionFill(0x0000FF, 500);
        delay(250);
        sideFill(0xFF0000, 500);
        delay(250);
      }
    } else {
      clearStrip();
    }
    
}
}

/************************ Animations for led strip ************************/
// alternate between two colors on even and odd LEDs
void alternateColor(uint32_t c1, uint32_t c2, uint8_t wait) {
  for(uint16_t i=0; i<ledStrip.numPixels(); i++) {
    if(i%2 == 0) { // set even LED to color 1
      ledStrip.setPixelColor(i, c1);
    } else { // set odd LED to color 2
      ledStrip.setPixelColor(i, c2);
    }
  }
 
  ledStrip.show(); // apply the colors
  delay(wait);
 
  for(uint16_t i=0; i<ledStrip.numPixels(); i++) {
    if(i%2 == 0) { // set even LED to color 2
      ledStrip.setPixelColor(i, c2);
    } else { // set odd LED to color 1
      ledStrip.setPixelColor(i, c1);
    }
  }
 
  ledStrip.show(); // apply the colors
  delay(wait);
}

// gradually fill up the strip with random colors
void randomColorFill(uint8_t wait) {
  clearStrip();
 
  for(uint16_t i=0; i<ledStrip.numPixels(); i++) { // iterate over every LED of the strip
    int r = random(0,255); // generate a random color
    int g = random(0,255);
    int b = random(0,255);
 
    for(uint16_t j=0; j<ledStrip.numPixels()-i; j++) { // iterate over every LED of the strip, that hasn't lit up yet
      ledStrip.setPixelColor(j-1, ledStrip.Color(0, 0, 0)); // turn previous LED off
      ledStrip.setPixelColor(j, ledStrip.Color(r, g, b)); // turn current LED on
      ledStrip.show(); // apply the colors
      delay(wait);
    }
  }
}

// pick a random LED to light up until entire strip is lit
void randomPositionFill(uint32_t c, uint8_t wait) {
  clearStrip();
 
  int used[ledStrip.numPixels()]; // array to keep track of lit LEDs
  int lights = 0; // counter
 
  for(int i = 0; i<ledStrip.numPixels(); i++){ // fill array with 0
    used[i] = 0;
  }
 
  while(lights<ledStrip.numPixels()-1) {
    int j = random(0,ledStrip.numPixels()-1); // pick a random LED
    if(used[j] != 1){ // if LED not already lit, proceed
      ledStrip.setPixelColor(j, c);
      used[j] = 1; // update array to remember it is lit
      lights++;
      ledStrip.show(); // display
      delay(wait);
    }
  }
}

// Light up the strip starting from the middle
void middleFill(uint32_t c, uint8_t wait) {
  clearStrip();
 
  for(uint16_t i=0; i<(ledStrip.numPixels()/2); i++) { // start from the middle, lighting an LED on each side
    ledStrip.setPixelColor(ledStrip.numPixels()/2 + i, c);
    ledStrip.setPixelColor(ledStrip.numPixels()/2 - i, c);
    ledStrip.show();
    delay(wait);
  }
 
  for(uint16_t i=0; i<(ledStrip.numPixels()/2); i++) { // reverse
    ledStrip.setPixelColor(i, ledStrip.Color(0, 0, 0));
    ledStrip.setPixelColor(ledStrip.numPixels() - i, ledStrip.Color(0, 0, 0));
    ledStrip.show();
    delay(wait);
  }
}

// Light up the strip starting from the sides
void sideFill(uint32_t c, uint8_t wait) {
  clearStrip();
 
  for(uint16_t i=0; i<(ledStrip.numPixels()/2); i++) { // fill strip from sides to middle
    ledStrip.setPixelColor(i, c);
    ledStrip.setPixelColor(ledStrip.numPixels() - i, c);
    ledStrip.show();
    delay(wait);
  }
 
  for(uint16_t i=0; i<(ledStrip.numPixels()/2); i++) { // reverse
    ledStrip.setPixelColor(ledStrip.numPixels()/2 + i, ledStrip.Color(0, 0, 0));
    ledStrip.setPixelColor(ledStrip.numPixels()/2 - i, ledStrip.Color(0, 0, 0));
    ledStrip.show();
    delay(wait);
  }
}

// Clear animation, turn off the led strip
void clearStrip() {
  for(int i=0; i<ledStrip.numPixels(); i++) {
    ledStrip.setPixelColor(i,0, 0, 0);
  }
  ledStrip.show();
}
