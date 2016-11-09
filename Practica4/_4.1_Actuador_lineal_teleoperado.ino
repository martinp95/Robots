#include <Servo.h>
#define X_pin A0 // pin analógico para leer eje X
const int PARADO = 90;
const int ALANTE = 180;
const int ATRAS = 0;
const int ALANTE_DESPACIO = 100;
const int ATRAS_DESPACIO = 80;

Servo servo;

void setup() {
  // put your setup code here, to run once:
  servo.attach(8);
}

void loop() {
  // put your main code here, to run repeatedly:
  int valorX = analogRead(X_pin);

  if (valorX < 100) {
    servo.write(ATRAS);
  } else if (valorX > 100 && valorX < 412) {
    servo.write(ATRAS_DESPACIO);
  } else if (valorX > 412 && valorX < 612) {
    servo.write(PARADO);
  } else if (valorX > 612 && valorX < 913) {
    servo.write(ALANTE_DESPACIO);
  } else if (valorX > 913) {
    servo.write(ALANTE);
  }
}
