#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Adafruit_NeoPixel.h>

//Avec Roballs
//const char* ssid = "ENSIM'Elec";                // SSID du Wifi (si avec téléphone damien: Honor 7X)
//const char* password = "milharausecours!2018";  // mot de passe du WiFi (si avec téléphone damien: 69f73dc8fd79)
//Avec Kiroulpa
//const char* ssid = "Kiroulpa";
//const char* password = "Milharausecours!2018";
//Avec MilhabotGaming
const char* ssid = "MilhabotGaming";
const char* password = "Milharausecours!2018";
//Pour avec téléphone Damien
//const char* ssid = "Honor 7X";
//const char* password = "69f73dc8fd79";

ESP8266WebServer server(80); // on instancie un serveur ecoutant sur le port 80

//LED strip declaration
const int NB_LED = 9;
const int PIN_LED = D3;
Adafruit_NeoPixel ledStrip = Adafruit_NeoPixel(NB_LED, PIN_LED); //number of LEDs, pin

void setup() {

  Serial.begin(9600);
  Serial.println("begin");
  /*led strip initialisation*/
  pinMode(PIN_LED, OUTPUT);
  ledStrip.begin();
  ledStrip.setBrightness(64); //0 to 255

  // on affiche l'adresse MAC de la carte
  Serial.println("");
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());
  
  /*wifi init*/
  WiFi.hostname("experience");
  // on demande la connexion au WiFi
  WiFi.begin(ssid, password);
  Serial.println("");
  // on attend d'etre connecte au WiFi avant de continuer
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // on affiche l'adresse IP qui nous a ete attribuee
  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // on definit ce qui doit etre fait lors de l'appel des routes (route = /on par exemple)

  // activation du bandeau de leds
  server.on("/on", [](){
    server.send(200, "text/plain", "Experience is ON");
    Serial.println("Experience is ON");
    //animations
    for(int i=0; i<5; i++) {
      alternateColor(0xFF0000, 0x0000FF, 500);
      delay(1000);
      randomColorFill(500);
      delay(1000);
      middleFill(0xFF000, 500);
      delay(1000);
      randomPositionFill(0x0000FF, 500);
      delay(1000);
      sideFill(0xFF0000, 500);
      delay(1000);
    }
    
  }); 

  // désactivation du bandeau de leds
  server.on("/off", [](){
    clearStrip();
    server.send(200, "text/plain", "Experience is OFF");
    Serial.println("Experience is OFF");
  });
  
  // on commence a ecouter les requetes venant de l'exterieur
  server.begin();
  Serial.println("HTTP server started");
  
}

void loop() {
  // on appelle le handle pour traiter les requêtes
  server.handleClient();
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
