#include <Servo.h>
Servo servoIzq;
Servo servoDer;
int NO_LINEA = 1;
int LINEA = 0;

int pinServoDer = 9;
int pinServoIzq = 8;
int pinIrDer = 3;
int pinIrIzq = 2;

struct Salida {
 boolean activo = false;
 int servoIzq = -1;
 int servoDer = -1;
};

Salida sComp1 ;
Salida sComp2 ;
Salida sComp3 ;
double cont = 0.0;
 
void setup() {
 Serial.begin(9600);
 servoIzq.attach(pinServoIzq);
 servoDer.attach(pinServoDer);
 pinMode( pinIrDer , INPUT);
 pinMode( pinIrIzq , INPUT);
}
void loop() {

 int valorSensorIzq = digitalRead(pinIrIzq);
 int valorSensorDer = digitalRead(pinIrDer);

 if (!sComp1.activo)
  cont = 0;
 

 comp1_EntratEnPista(valorSensorIzq,valorSensorDer);
 if ( sComp1.activo ){
 //Serial.println("Comp1 ACTIVO ");
 //Serial.println("ServoIzq:"+String(sComp1.servoIzq));
 //Serial.println("ServoDer:"+String(sComp1.servoDer));
 servoIzq.write(sComp1.servoIzq);
 servoDer.write(sComp1.servoDer);
 
 return;
 }

 comp2_CorregirTrayectoria(valorSensorIzq,valorSensorDer);
 if ( sComp2.activo ){
 //Serial.println("Comp2 ACTIVO ");
 //Serial.println("ServoIzq:"+String(sComp2.servoIzq));
 //Serial.println("ServoDer:"+String(sComp2.servoDer));
 servoIzq.write(sComp2.servoIzq);
 servoDer.write(sComp2.servoDer);
 return;
 }

 comp3_Avanzar(valorSensorIzq,valorSensorDer);
 if ( sComp3.activo ){
 //Serial.println("Comp3 ACTIVO ");
 //Serial.println("ServoIzq:"+String(sComp3.servoIzq));
 //Serial.println("ServoDer:"+String(sComp3.servoDer));
 servoIzq.write(sComp3.servoIzq);
 servoDer.write(sComp3.servoDer);
 return;
 } 
}

void comp1_EntratEnPista(int valorSensorIzq,int valorSensorDer){
 sComp1.activo = false;
 if( valorSensorIzq == NO_LINEA && valorSensorDer == NO_LINEA ){
 cont = cont + 0.0003;
 sComp1.activo = true;
 sComp1.servoIzq = 90.0- cont;
 sComp1.servoDer = 180;
 }
}
void comp2_CorregirTrayectoria(int valorSensorIzq,int valorSensorDer){
 sComp2.activo = false;
 if( valorSensorIzq == LINEA && valorSensorDer == NO_LINEA ){
 sComp2.activo = true;
 sComp2.servoIzq = 0;
 sComp2.servoDer = 90;
 
 }
 if( valorSensorIzq == NO_LINEA && valorSensorDer == LINEA ){
 sComp2.activo = true;
 sComp2.servoIzq = 90;
 sComp2.servoDer = 180;
 }
}
void comp3_Avanzar(int valorSensorIzq,int valorSensorDer){
 sComp3.activo = false;
 if( valorSensorIzq == LINEA && valorSensorDer == LINEA){
 sComp3.activo = true;
 sComp3.servoIzq = 0;
 sComp3.servoDer = 180;
 }
}


