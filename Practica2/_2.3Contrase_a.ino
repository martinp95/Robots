#include <Keypad.h>
#include <Servo.h>

Servo servo1;

const byte nfilas = 4;
const byte ncolumnas = 4;
char teclas[nfilas][ncolumnas] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pfilas[nfilas] = {2, 3, 4, 5}; // Filas
byte pcolumnas[ncolumnas] = {A0, A1, A2, A3}; //Columnas
Keypad teclado = Keypad( makeKeymap(teclas), pfilas, pcolumnas, nfilas, ncolumnas );


const int abierto = 90;
const int cerrado = 0;

int led = 13;

//Constraseña
const String password = "5412";

String bufferLectura = "";

void setup() {
  Serial.begin(9600);
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  servo1.attach(8);
  servo1.write(cerrado);
}

// the loop routine runs over and over again forever:
void loop() {
  char tecla = teclado.getKey();
  if (bufferLectura.length() == 4) {
    Serial.print(bufferLectura + "\n");
    if (bufferLectura == password) {
      Serial.print("Contraseña correcta\n");
      digitalWrite(led, HIGH);
      bufferLectura = "";
      servo1.write(abierto);
      delay(1000);
      servo1.write(cerrado);
      digitalWrite(led, LOW);
    } else {
      Serial.print("Contraseña incorrecta\n");
      parpadearLed();
      bufferLectura = "";
    }

  } else {
    if (tecla != '\0') {
      bufferLectura += tecla;
      Serial.print(bufferLectura + "\n");
    }
  }
}

void parpadearLed() {
  digitalWrite(led, HIGH);
  delay(300);
  digitalWrite(led, LOW);
  delay(300);
  digitalWrite(led, HIGH);
  delay(300);
  digitalWrite(led, LOW);
  delay(300); digitalWrite(led, HIGH);
  delay(300);
  digitalWrite(led, LOW);
}
