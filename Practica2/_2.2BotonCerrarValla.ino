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

bool barreraAbierta = false;
const int abierto = 90;
const int cerrado = 0;

double inicioCuentaTiempo;

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  servo1.attach(8);
  servo1.write(cerrado);
}

void loop() {
  char tecla = teclado.getKey();
  if (tecla != '\0') {
    Serial.print(tecla);
    Serial.print("\n");
    if (tecla == 'A' && !barreraAbierta) {
      barreraAbierta = true;
      inicioCuentaTiempo = millis();
      Serial.print("Abriendo valla.\n");
      servo1.write(abierto);
    } else if (tecla == 'C' && barreraAbierta) {
      barreraAbierta = false;
      servo1.write(cerrado); 
      Serial.print("Cerrando valla.\n");
    }
  }
  if ( millis() - inicioCuentaTiempo >= 5000 && barreraAbierta) {
    barreraAbierta = false;
    servo1.write(cerrado); 
    Serial.print("Cerrando valla.\n");
  }
}
