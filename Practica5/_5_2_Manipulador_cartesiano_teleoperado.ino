#include <Servo.h> 

// Grados pinza, MAX y MIN admitido
#define GMAX 100
#define GMIN 10

// JOYSTICK PIN
#define pin_Pinza_Joy A0 
#define pin_Pinza_Servo 8 

// JOYSTICK PIN
#define pin_X_Joy A1
#define pin_Y_Joy A2  

#define pin_Xservo 7
#define pin_Yservo 6

Servo servoP;
Servo servoX;
Servo servoY;

int gradosPinza = GMIN;

void setup() {
  Serial.begin(9600);
  servoP.attach(pin_Pinza_Servo);
  servoX.attach(pin_Xservo);
  servoY.attach(pin_Yservo);
  
  servoP.write(gradosPinza);
  //TODO
}

void loop() {
  int pinza = analogRead(pin_Pinza_Joy);
  int x = analogRead(pin_X_Joy);
  int y = analogRead(pin_Y_Joy);
  
  //Serial.println("P: "+String(pinza));
  Serial.println("X: "+String(x));
  Serial.println("Y: "+String(y));
  
  moverPinza(pinza);
  moverEjeXY(x, y);

}

void moverPinza(int X){
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


  void moverEjeXY(int valorX, int valorY){
  
    if(valorX >600)
      servoX.write(0);
    else if(valorX < 400)
      servoX.write(180);
    else 
      servoX.write(90);


  
    if(valorY >600)
       servoY.write(0);
    else if(valorY < 400)
       servoY.write(180);  
    else
       servoY.write(90);
  }
