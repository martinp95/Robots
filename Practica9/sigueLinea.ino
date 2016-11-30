#include <Servo.h>
Servo servoIzq;
Servo servoDer;
int NO_LINEA = 1;
int LINEA = 0;

int pinServoDer = 9;
int pinServoIzq = 8;
int pinIrDer = 3;
int pinIrIzq = 2;



void setup() {
  servoIzq.attach(pinServoIzq);
  servoDer.attach(pinServoDer);
  pinMode( pinIrDer , INPUT);
  pinMode( pinIrIzq , INPUT);

}

void loop() {
 //servoIzq.write(0); // Vel Máxima
// servoDer.write(180);
 
 if (digitalRead(pinIrDer) == LINEA && digitalRead(pinIrIzq) == LINEA){
  servoIzq.write(0);
  servoDer.write(180);
 }

 if (digitalRead(pinIrDer) == NO_LINEA && digitalRead(pinIrIzq) == NO_LINEA){
  servoIzq.write(90);
  servoDer.write(90);
 }

 if (digitalRead(pinIrDer) == NO_LINEA && digitalRead(pinIrIzq) == LINEA){
  servoIzq.write(0);
  servoDer.write(90);
 }

 if (digitalRead(pinIrDer) == LINEA && digitalRead(pinIrIzq) == NO_LINEA){  
  servoIzq.write(90);
  servoDer.write(180); 

 }
}

