 #include <Wire.h>

//Moteur Gauche
#define enA_Gauche 6  //Marron
#define in2_Gauche 10 //Inutile
#define in1_Gauche 7 //Noire

//Moteur Droite
#define enB_Droite 9   //Jaune
#define in4_Droite 8  //Inutile
#define in3_Droite 11  //Vert

#define ADDR_ARDUINO 8

int PWM_Gauche, PWM_Droite; // 0 ... 255
int sens_Gauche, sens_Droite; // 0 = avant / 1 = arriere

void setup() {
  Serial.begin(115200);
  Wire.begin(ADDR_ARDUINO);
  Wire.onReceive(recv);

  PWM_Gauche = 0; PWM_Droite = 0;
  sens_Gauche = 0; sens_Droite = 0;

  pinMode(enA_Gauche, OUTPUT);
  pinMode(enB_Droite, OUTPUT);
  pinMode(in1_Gauche, OUTPUT);
  pinMode(in2_Gauche, OUTPUT);
  pinMode(in3_Droite, OUTPUT);
  pinMode(in4_Droite, OUTPUT);

  //arrêt des moteurs
  digitalWrite(in1_Gauche, LOW);
  digitalWrite(in2_Gauche, LOW);
  digitalWrite(in3_Droite, LOW);
  digitalWrite(in4_Droite, LOW);

  analogWrite(enA_Gauche, PWM_Gauche);
  analogWrite(enB_Droite, PWM_Droite);
  //INSA RENNES 
  TCCR0B = (TCCR0B & 0b11111000) | 1;
  TCCR1B = (TCCR1B & 0b11111000) | 1;
  TCCR2B = (TCCR2B & 0b11111000) | 1;
  
  //Serial.println("STARTING");
}

void loop() { delay(1); }

void recv(int numBytes) {
  if(numBytes == 4) {
    PWM_Gauche = Wire.read();
    PWM_Droite = Wire.read();
    sens_Gauche = Wire.read();
    sens_Droite = Wire.read();

    //Serial.print("Gauche -> "); Serial.print((sens_Gauche == 2 ? "marche arriere : " : (sens_Gauche == 1 ? "marche avant : " : "arret : "))); Serial.println(PWM_Gauche);
    //Serial.print("Droite -> "); Serial.print((sens_Droite == 2 ? "marche arriere : " : (sens_Droite == 1 ? "marche avant : " : "arret : "))); Serial.println(PWM_Droite);

    Serial.print("Gauche -> (");
    Serial.print(sens_Gauche);
    Serial.print(") ");
    switch(sens_Gauche) {
      case 1: //Roue Gauche Marche Avant
        digitalWrite(in1_Gauche, LOW);
        digitalWrite(in2_Gauche, HIGH);
        //Serial.print("marche avant : ");
        break;
      case 2: //Roue Gauche Marche Arriere
        digitalWrite(in1_Gauche, HIGH);
        digitalWrite(in2_Gauche, LOW);
        //Serial.print("marche arriere : ");
        break;
      case 0:
      default: //Arrêt du moteur (frein)
        digitalWrite(in1_Gauche, HIGH);
        digitalWrite(in2_Gauche, HIGH);
        //Serial.print("arret : ");
        break;
    }
    //Serial.println(PWM_Gauche);
    
    //Serial.print("Droite -> (");
    //Serial.print(sens_Droite);
    //Serial.print(") ");
    switch(sens_Droite) {
      case 1: //Roue Droite Marche Avant
        digitalWrite(in3_Droite, LOW);
        digitalWrite(in4_Droite, HIGH);
        //Serial.print("marche avant : ");
        break;
      case 2: //Roue Droite Marche Arriere
        digitalWrite(in3_Droite, HIGH);
        digitalWrite(in4_Droite, LOW);
        //Serial.print("marche arriere : ");
        break;
      case 0:
      default: //Arrêt du moteur (frein)
        digitalWrite(in3_Droite, HIGH);
        digitalWrite(in4_Droite, HIGH);
        //Serial.print("arret : ");
        break;
    }
    //Serial.println(PWM_Droite);
    //Serial.print("\n");
    analogWrite(enA_Gauche, PWM_Gauche);
    analogWrite(enB_Droite, PWM_Droite);
  }
  //Stream.flush();
  while(Wire.available()) Wire.read();
}


