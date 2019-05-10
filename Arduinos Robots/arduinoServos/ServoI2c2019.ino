#include <Wire.h>
#include <Servo.h>
#include <avr/wdt.h>

#define ADDR_ARDUINO 6
#define SERVO_PIN2 2
#define SERVO_PIN3 3
#define SERVO_PIN4 4
#define SERVO_PIN5 5
#define SERVO_PIN6 6
#define SERVO_PIN7 7
#define SERVO_PIN8 8
#define SERVO_PIN9 9
#define SERVO_PIN10 10
#define SERVO_PIN11 11
#define SERVO_PIN12 12
#define SERVO_PIN13 13

int servoN;
int rotation;

uint8_t data[6];
Servo s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13;

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin(ADDR_ARDUINO);
  Wire.flush();
  Wire.onReceive(recv);
  Wire.onRequest(sendData);
  //Wire.onRequest(sendData);
  s2.attach(SERVO_PIN2);
  s3.attach(SERVO_PIN3);
  s4.attach(SERVO_PIN4);
  s5.attach(SERVO_PIN5);
  s6.attach(SERVO_PIN6);
  s7.attach(SERVO_PIN7);
  s8.attach(SERVO_PIN8);
  s9.attach(SERVO_PIN9);
  s10.attach(SERVO_PIN10);
  s11.attach(SERVO_PIN11);
  s12.attach(SERVO_PIN12);
  s13.attach(SERVO_PIN13);
  s2.write(135 ); //gauche
  s3.write(15); //droite
  s4.write(0);
  s5.write(0);
  s6.write(0);  
  s7.write(0);  
  s8.write(0); 
  s9.write(0); 
  s10.write(0); 
  s11.write(0); 
  s12.write(0);
  s13.write(0);
  Serial.println("STARTING");
}

void sendData() {

  Wire.write(72);

}

void loop() 
{
  // put your main code here, to run repeatedly:
}
void recv(int bytes)
{
     wdt_reset();
     while(Wire.available()) 
     {
      servoN = Wire.read();
      rotation = Wire.read();
      //Serial.print("Donnee recue : ");
      //Serial.print(servoN);
      //Serial.print(" rotation = ");
      //Serial.print(rotation);
      //Serial.print("\n");
      switch (servoN)
      {
        case 2 :
          s2.write(rotation);
        break;
        case 3 :
           s3.write(rotation);
        break;
        case 4 :
           s4.write(rotation);
        break;
        case 5 :
           s5.write(rotation);
        break;
        case 6 :
           s6.write(rotation);
        break;
        case 7 :
           s7.write(rotation);
        break;
        case 8 :
           s8.write(rotation);
        break;
        case 9 :
           s9.write(rotation);
        break;
        case 10 :
           s10.write(rotation);
        break;
        case 11 :
           s11.write(rotation);
        break;
        case 12 :
           s12.write(rotation);
        break;
        case 13 :
           s13.write(rotation);
        break;
      }
     }
     
    
}
