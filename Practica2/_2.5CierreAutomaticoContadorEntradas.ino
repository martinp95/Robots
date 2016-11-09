#include <Servo.h>

Servo servo1;

const int abierto = 90;
const int cerrado = 0;

const int OSCURO = 180;

const int sensorDistancia = 0;
const int sensorLuz = 1;

int sensorDeteccion;


long tiempoRespuesta;

int pinTrig = 9;
int pinEcho = 8;

int pinServo = 7;

int pinSensorLuz = A4;

bool barreraAbierta = false;

int numeroEntradas;
int numeroSalidas;

double inicioCuentaTiempo;

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  servo1.attach(pinServo);
  servo1.write(cerrado);
  pinMode(pinTrig , OUTPUT); /* trig envía el pulso ultrasónico */
  pinMode(pinEcho, INPUT); /* Echo capta el rebote del pulso ultrasónico*/
  numeroEntradas = 0;
  numeroSalidas = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!barreraAbierta) {
    if (distancia() < 5) {
      sensorDeteccion = 0;
      servo1.write(abierto);
      barreraAbierta = true;
      inicioCuentaTiempo = millis();
      Serial.print("Detectada Entrada\n");

    } else if (valorLuz() < OSCURO) {
      sensorDeteccion = 1;
      servo1.write(abierto);
      barreraAbierta = true;
      inicioCuentaTiempo = millis();
      Serial.print("Detectada Salida\n");
    }
  } else {
    if (sensorDeteccion == 0 && valorLuz() < OSCURO) {
      servo1.write(cerrado);
      barreraAbierta = false;
      numeroEntradas = numeroEntradas + 1;
      Serial.print("Numero de entradas: ");
      Serial.print(numeroEntradas);
      delay(1000);
    } else if (sensorDeteccion == 1 && distancia() < 5) {
      servo1.write(cerrado);
      barreraAbierta = false;
      numeroSalidas += 1;
      Serial.print("Numero de salidas: ");
      Serial.print(numeroSalidas);
      delay(1000);
    }
    delay(300);
  }

  if(isTiempoSobrepasado()){
    cerrarVaya();
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

int valorLuz() {
  int valorLuz = analogRead(pinSensorLuz);
 // Serial.print(valorLuz);
 // Serial.print("\n");
  return valorLuz;
}

bool isTiempoSobrepasado(){
  if ( millis() - inicioCuentaTiempo >= 5000 && barreraAbierta) {
    return true;
  }
  return false;
}

void cerrarVaya(){
    barreraAbierta = false;
    servo1.write(cerrado); 
    Serial.print("Cerrando valla.\n");
}

