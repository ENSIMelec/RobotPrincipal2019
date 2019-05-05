#include <Wire.h>
 /*
 * M1 = MG
 * M2 = MD
 * in1 = AV
 * in2 = ARR
 */
#define MGinARR 6//moteur gauche arrière
#define MGinAV  9 //moteur gauche avant
#define MDinAV  10  //moteur droit avant 
#define MDinARR 11  //moteur droit arrière

#define AV 1 //indexs pour les tableaux de retour de la fonction de conversion
#define ARR 0

#define END 7
#define ENG 8

#define GAUCHE 0 
#define DROIT 1

#define ADDR_ARDUINO    8

#define CMD_RESET 1
#define DISABLE_WATCHDOG 2
#define ENABLE_WATCHDOG 3



 int commandG[2];
 int commandD[2];

volatile int nbTicD; //compteur de ticks codeur droit
volatile int nbTicG; // compteur de ticks codeur gauche 

volatile bool cmdRcv = false;
volatile int8_t cmdM[2];

void setup()
{
  Serial.begin(9600);
  Wire.begin(ADDR_ARDUINO);//adresse i2c en 8
  Wire.onReceive(recv);
  //Wire.onRequest(sendData);
  
  pinMode(MGinARR, OUTPUT);
  pinMode(MDinARR, OUTPUT);
  pinMode(MGinAV, OUTPUT);
  pinMode(MDinAV, OUTPUT);
  pinMode(END, OUTPUT);
  pinMode(ENG, OUTPUT);
  pinMode(13, OUTPUT);


  digitalWrite(END, HIGH);
  digitalWrite(ENG, HIGH);
  
  //digitalWrite(13, HIGH);
  Serial.print("STARTING");
}
  //int i = 0;
void loop()
{
  delay(5000);
  //Serial.println(Distance_ultason(Trig1,Echo1));
}


void gauche(int inAV, int inARR){
 
   analogWrite(MGinAV, inAV);
  analogWrite(MGinARR, inARR);

 }
 
void droit(int inAV, int inARR){
  
   analogWrite(MDinAV, inAV);
  analogWrite(MDinARR, inARR);

 }
 
 

 void convertI2cEngineCommandToAnalogCommand(uint8_t i2c, int* res)
 {
   /*
   * 
    * marche arrière 0..127 → 255..1
  * stop 128 → 0
    * marche avant 129..255 → 0..255
  */
   //Serial.print("\nreçu");
    //Serial.print(i2c);
     int tmp = i2c - 128;
   res[ARR] = tmp < 0 ? -tmp*2 -1: 0;
   //Serial.print("\nMoteur arrière :");
  //Serial.print(res[ARR]);
    res[AV] = tmp > 0 ? tmp*2 +1: 0;
 

     //Serial.print("\nMoteur avant :");
     //Serial.print(res[AV]);
 }

  bool isBetween(int min, int val, int max)
 {
        if(val < min || val > max)
                return false;
        return true;
 }
 

 void recv(int bytes){//valeurs des moteurs reçues de la rasp
    /*
     * recéption moteur gauche puis droit
     */
 
  
    //delay(150);     
    //digitalWrite(13, HIGH);    
   //Serial.print("Donnee recue : ");
  
     if(bytes == 2)
     {
       //Serial.print("Donnee recue : ");
      /* ajouté dans l'interruption dsans cette version (précédemment dans le main) */

      convertI2cEngineCommandToAnalogCommand(Wire.read(), commandG);
      convertI2cEngineCommandToAnalogCommand(Wire.read(), commandD);
      if(isBetween(0, commandG[AV], 255) && isBetween(0, commandG[ARR], 255))
         gauche(commandG[AV], commandG[ARR]);
      else
        gauche(128, 128);
         
      if(isBetween(0, commandD[AV], 255) && isBetween(0, commandD[ARR], 255))
          droit(commandD[AV], commandD[ARR]);
      else
         droit(128, 128);
     }

     while(Wire.available())Wire.read();
    
 
 
  //digitalWrite(13, LOW);   
     
 }
