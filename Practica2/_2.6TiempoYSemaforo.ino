#include <Servo.h>
#include <Keypad.h>

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

bool configAdmitido;
String bufferLectura = "";
int tiempoAbierta;

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

bool barreraAbierta;

int numeroEntradas;
int numeroSalidas;

double inicioCuentaTiempo;

int ledVerde = 12;
int ledRojo = 11;
int ledAzul = 10;

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  servo1.attach(pinServo);
  servo1.write(cerrado);
  pinMode(pinTrig , OUTPUT); /* trig envía el pulso ultrasónico */
  pinMode(pinEcho, INPUT); /* Echo capta el rebote del pulso ultrasónico*/
  numeroEntradas = 0;
  numeroSalidas = 0;
  barreraAbierta = false;
  tiempoAbierta = 5000;
  configAdmitido = false;
  pinMode(ledVerde , OUTPUT);
  pinMode(ledRojo , OUTPUT);
  pinMode(ledAzul , OUTPUT);
  digitalWrite(ledVerde,HIGH);
}

void loop() {
  char tecla = teclado.getKey();
  if (tecla != '\0') {
    Serial.println("tecla pulsada: " + String(tecla));
    configurarTiempo(tecla);
  }

  if (!barreraAbierta) {
    if (distancia() < 5) {
      sensorDeteccion = 0;
      servo1.write(abierto);
      barreraAbierta = true;
      digitalWrite(ledRojo,HIGH);
      digitalWrite(ledVerde,LOW);
      inicioCuentaTiempo = millis();
      Serial.print("Detectada Entrada\n");

    } else if (valorLuz() < OSCURO) {
      sensorDeteccion = 1;
      servo1.write(abierto);
      barreraAbierta = true;
      digitalWrite(ledAzul,HIGH);
      digitalWrite(ledVerde,LOW);
      inicioCuentaTiempo = millis();
      Serial.print("Detectada Salida\n");
    }
  } else {
    if (sensorDeteccion == 0 && valorLuz() < OSCURO) {
      servo1.write(cerrado);
      barreraAbierta = false;
      digitalWrite(ledRojo,LOW);
      digitalWrite(ledVerde,HIGH);
      numeroEntradas = numeroEntradas + 1;
      Serial.print("Numero de entradas: ");
      Serial.print(numeroEntradas);
      Serial.print("\n");
      delay(1000);
    } else if (sensorDeteccion == 1 && distancia() < 5) {
      servo1.write(cerrado);
      barreraAbierta = false;
      digitalWrite(ledAzul,LOW);
      digitalWrite(ledVerde,HIGH);
      numeroSalidas += 1;
      Serial.print("Numero de salidas: ");
      Serial.print(numeroSalidas);
      Serial.print("\n");
      delay(1000);
    }
  }

  if (isTiempoSobrepasado()) {
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
  return valorLuz;
}

bool isTiempoSobrepasado() {
  if ( millis() - inicioCuentaTiempo >= tiempoAbierta && barreraAbierta) {
    return true;
  }
  return false;
}

void cerrarVaya() {
  barreraAbierta = false;
  digitalWrite(ledRojo,LOW);
  digitalWrite(ledAzul,LOW);
  digitalWrite(ledVerde,HIGH);
  servo1.write(cerrado);
  Serial.print("Cerrando valla.\n");
}

/*
   Metodo para cambiar el tiempo que permanece abierta la barrera.
*/
void configurarTiempo(char tecla) {
  if (tecla != '*' && configAdmitido) {
    bufferLectura = bufferLectura + tecla;
    Serial.println("buffer: " + bufferLectura);
  } else if (tecla == '*' && !configAdmitido) { // es * transformamos a entero
    configAdmitido = true;
  } else if (tecla == '*' && configAdmitido) {
    tiempoAbierta = bufferLectura.toInt();
    bufferLectura = ""; // reinicio
    configAdmitido = false;
  }
}
