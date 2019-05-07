
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

byte relON[] = {0xA0, 0x01, 0x01, 0xA2};  //Hex command to send to serial for open relay
byte relOFF[] = {0xA0, 0x01, 0x00, 0xA1}; //Hex command to send to serial for close relay

//Avec Roballs
//const char* ssid = "ENSIM'Elec";                // SSID du Wifi (si avec téléphone damien: Honor 7X)
//const char* password = "****";  // mot de passe du WiFi (si avec téléphone damien: 69f73dc8fd79)
//Avec Kiroulpa
//const char* ssid = "Kiroulpa";
//const char* password = "****";
//Avec MilhabotGaming
const char* ssid = "MilhabotGaming";
const char* password = "****";
//Pour avec téléphone Damien
//const char* ssid = "Honor 7X";
//const char* password = "69f73dc8fd79";

ESP8266WebServer server(80); // on instancie un serveur ecoutant sur le port 80

void setup() {

  // on fixe la vitesse de la communication avec la carte relais
  Serial.begin(9600);

  WiFi.hostname("electron");
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
  
  // on definit ce qui doit etre fait lors de l'appel des routes (route = /on)

  // activation du relais
  server.on("/on", [](){
    Serial.write(relON, sizeof(relON));
    server.send(200, "text/plain", "motor is ON");
    Serial.println("motor is ON");
  }); 

  // désactivation du relais
  server.on("/off", [](){
    Serial.write(relOFF, sizeof(relOFF));
    server.send(200, "text/plain", "motor is OFF");
    Serial.println("motor is OFF");
  });
  
  // on commence a ecouter les requetes venant de l'exterieur
  server.begin();
  Serial.println("HTTP server started");

}

void loop(void){
  // on appelle le handle pour traiter les requêtes
  server.handleClient();
}
