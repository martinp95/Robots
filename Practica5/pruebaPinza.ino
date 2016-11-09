#include <Servo.h> 

// Grados pinza, MAX y MIN admitido
#define GMAX 110
#define GMIN 10

// JOYSTICK PIN
#define pin_X A0 

Servo servoP;
int gradosPinza = GMIN;

void setup() {
  Serial.begin(9600);
  servoP.attach(4);
  servoP.write(gradosPinza);
}

void loop() {
  int X = analogRead(pin_X);
  Serial.println("P: "+String(X));

  // Aumentar / reducir grados segun X-Joystick
  // Joystick de 0 a 1024, desechamos las posiciones centrales
  if(X > 700 ){
      if ( gradosPinza < GMAX)
         gradosPinza = gradosPinza+1;
  } else if ( X < 300){
      if ( gradosPinza > GMIN)
        gradosPinza = gradosPinza-1;
  } 
  
  // Mover
  servoP.write(gradosPinza);
  Serial.println("G: "+String(gradosPinza));

}
