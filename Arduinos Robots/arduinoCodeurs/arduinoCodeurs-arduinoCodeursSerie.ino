#include <Wire.h> 
#include <avr/io.h>

#define interCodG  0 //pin2 (interrupt0) 
#define interCodD  1 //pin3 (interrup1) 
#define AV 1 //indexs pour les tableaux de retour de la fonction de conversion (normalement inutilisé mais on va les garder pour la norme) 
#define ARR 0 
#define GAUCHE 0 
#define DROIT 1 
 
#define PIN_ATT 5 
 
 
#define CANAL_BD  8 //canal avant ou arrière pour les codeurs (droit) 
#define CANAL_BG  7 // (gauche) 
 
#define ADDR_ARDUINO  9 
 
#define CMD_RESET_ACTIONNEURS -1 
#define CMD_RESET_CODEURS -3 
#define CMD_ACK -2 
 
 
 
volatile long nbTicD; //compteur de ticks codeur droit 
volatile long nbTicG; // compteur de ticks codeur gauche 
volatile long temps;
 
void setup() 
{ 
   Serial.begin(115200); 
  //Wire.begin(ADDR_ARDUINO);//adresse i2c en 9 pour les codeurs 
  //Wire.onRequest(rqst);//reception i2c 
  //Wire.onReceive(recv); 
 
  reset(); 
  attachInterrupt(interCodD, compterTicDroit, RISING); 
  attachInterrupt(interCodG, compterTicGauche, RISING); 
}
 
 
void reset()
{
  nbTicD = 0;
  nbTicG = 0;
  temps = micros();
}
 
 
void loop() 
{ 
   if (Serial.available())  
  { 
    int data = Serial.read(); 
   switch (data) 
   { 
      case 'C' : 
        temps = micros(); 
        Serial.print("?"); 
        Serial.print(nbTicG); 
        Serial.print(","); 
        Serial.print(nbTicD); 
        Serial.print(":"); 
        Serial.print(temps); 
        Serial.print(";"); 
      break; 
 
      case 'R' : 
         reset(); 
      break; 
   }    
      
  } 
   
} 
 
 
void compterTicDroit ()
{
  //if (digitalRead(CANAL_BD) == HIGH)
  if (PINB & 0x01)
  {
    nbTicD--;
  }
  else
    nbTicD++;
}
 
void compterTicGauche ()
{
  //if (digitalRead(CANAL_BG) == HIGH)
  if(PIND & 0x80)
  {
    nbTicG++;
     }
  else
    nbTicG--;
}
