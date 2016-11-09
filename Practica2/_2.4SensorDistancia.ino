#include <Servo.h>

Servo servo1;

const int abierto = 90;
const int cerrado = 0;


long tiempoRespuesta;

int pinTrig = 9;
int pinEcho = 8;

bool barreraAbierta = false;

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  servo1.attach(7);
  servo1.write(cerrado);
  pinMode(pinTrig , OUTPUT); /* trig envía el pulso ultrasónico */
  pinMode(pinEcho, INPUT); /* Echo capta el rebote del pulso
   ultrasónico*/
}

void loop() {
  // put your main code here, to run repeatedly:
  if (distancia() < 5) {
    if (!barreraAbierta) {
      servo1.write(abierto);
      barreraAbierta = true;
      delay(5000);
    }
  } else if (barreraAbierta) {
    servo1.write(cerrado);
    barreraAbierta = false;
  }
}

long distancia() {
  long distancia;
  digitalWrite(pinTrig, LOW); /* Por seguridad volvemos a poner el Trig
  a LOW*/
  delayMicroseconds(5);

  digitalWrite(pinTrig, HIGH); /* Emitimos el pulso ultrasónico */
  delayMicroseconds(10);

  tiempoRespuesta = pulseIn(pinEcho, HIGH); /* Medimos la longitud del
  pulso entrante Cuanto tiempo tarda la entrada en pasar de HIGH a LOW
  retorna microsegundos */

  distancia = int(0.017 * tiempoRespuesta); /* Calcular la distancia
  conociendo la velocidad */

  return distancia;
}

