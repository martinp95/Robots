#include <Servo.h>
#define botonTopeDerecha 5
#define botonTopeIzquierda 6
#define botonTopeArriba 7
#define botonTopeAbajo 8
#define pinServoX 9
#define pinServoY 10
#define pinPinza 11

// Grados pinza, MAX y MIN admitido
#define GMAX 5
#define GMIN 94

Servo servoX, servoY, pinza;

int gradosPinza = GMIN;

//Constantes movimiento servo motor
const int GIRO_HORARIO = 180;
const int GIRO_ANTI_HORARIO = 0;
const int PARADO = 90;

//Variables que guardan el tiempo que tarda el carro en recorrer el ejeX
long tiempoEjeX, tiempoInicioEjeX, tiempoFinEjeX;

//Variables que guardan el tiempo que tarda el carro en recorrer el ejeY
long tiempoEjeY, tiempoInicioEjeY, tiempoFinEjeY;

//Array donde se guardan las instrucciones de alto nivel.
String instrucciones[24];

//Posicion actual del carro en el eje X y en el eje Y.
int coordenadaActualEjeX, coordenadaActualEjeY;

//Variables control en el eje X
bool topeDerecha;
bool topeIzquierda;

//Varibles control en el eje Y
bool topeArriba;
bool topeAbajo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servoX.attach(pinServoX);
  servoY.attach(pinServoY);
  pinza.attach(pinPinza);
  pinMode(botonTopeDerecha, INPUT);
  pinMode(botonTopeIzquierda, INPUT);
  pinMode(botonTopeArriba, INPUT);
  pinMode(botonTopeAbajo, INPUT);

  pinza.write(gradosPinza);

  //calibrado de los carros
  tiempoEjeX = calibrarEjeX();
  tiempoEjeY = calibrarEjeY();

  coordenadaActualEjeX = 0;
  coordenadaActualEjeY = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    //Generar array de instrucciones
    generarArrayInstrucciones();
    //Empezar a mover los carros segun las instrucciones lo indiquen
    cargarInstrucciones();
  }
}

void cargarInstrucciones() {
  int i = 0;
  while (instrucciones[i] != "" && i < 24) {
    interpretarInstruccion(instrucciones[i]);
    i++;
  }
}

/*
   Metodo con un if inmenso donde se decide la forma de ejecutar la instruccion
   se podria optimizar usando objetos como en diseño de software.
*/
void interpretarInstruccion(String instruccion) {
  //tantos metodos ejecutar como tipos de instrucciones se tengan
  int coordenadaX, coordenadaY;
  if (instruccion.startsWith("MOVPY")) {
    //llamda al metodo movpy
    coordenadaX = instruccion.substring(instruccion.indexOf("(") + 1, instruccion.indexOf(",")).toInt();
    coordenadaY = instruccion.substring(instruccion.indexOf(",") + 1, instruccion.lastIndexOf(")")).toInt();;
    movpy(coordenadaX, coordenadaY);
  } else if (instruccion.startsWith("MOVPX")) {
    //llamada al metodo movpx
    coordenadaX = instruccion.substring(instruccion.indexOf("(") + 1, instruccion.indexOf(",")).toInt();
    coordenadaY = instruccion.substring(instruccion.indexOf(",") + 1, instruccion.lastIndexOf(")")).toInt();
    movpx(coordenadaX, coordenadaY);
  } else if (instruccion.startsWith("APINZA")) {
    apinza();
  } else if (instruccion.startsWith("CPINZA")) {
    cpinza();
  } else if (instruccion.startsWith("WAIT")) {
    //lamada al metodo wait
    long tiempo = instruccion.substring(instruccion.indexOf("(") + 1, instruccion.indexOf(")")).toInt();
    wait(tiempo);
  }
}

/*
   Muevo el robot primero en el eje Y hasta la coordenada Y y despues en el eje X hast ala coordenada X
*/
void movpy(int coordenadaX, int coordenadaY) {
  moverY(coordenadaY);
  moverX(coordenadaX);
}

/*
   Muevo el robot primero en el eje X hasta la coordenadaX y despues en el eje Y hasta la coordenadaY
*/
void movpx(int coordenadaX, int coordenadaY) {
  moverX(coordenadaX);
  moverY(coordenadaY);
}

/*
   Metodo que mueve el carro en el eje X a la coordenada pasada como parametro
*/
void moverX(int coordenadaX) {
  if (coordenadaX > coordenadaActualEjeX) {
    servoX.write(GIRO_HORARIO);
    delay(tiempoEjeX / 24 * (coordenadaX - coordenadaActualEjeX));
    servoX.write(PARADO);
  } else if (coordenadaX < coordenadaActualEjeX) {
    servoX.write(GIRO_ANTI_HORARIO);
    delay(tiempoEjeX / 24 * abs(coordenadaX - coordenadaActualEjeX));
    servoX.write(PARADO);
  }
  coordenadaActualEjeX = coordenadaX;
}

void moverY(int coordenadaY) {
  if (coordenadaY > coordenadaActualEjeY) {
    servoY.write(GIRO_HORARIO);
    delay(tiempoEjeX / 24 * (coordenadaY - coordenadaActualEjeY));
    servoY.write(PARADO);
  } else {
    servoY.write(GIRO_ANTI_HORARIO);
    delay(tiempoEjeX / 24 * abs(coordenadaY - coordenadaActualEjeY));
    servoY.write(PARADO);
  }
  coordenadaActualEjeY = coordenadaY;
}

/*
   Abre la pinza los grados son conocidos por el robot
*/
void cpinza() {
  while (gradosPinza < GMAX) {
    gradosPinza += 1;
    pinza.write(gradosPinza);
  }
}

/*
   Cierra la pinza, los grados son conocidos por el robot(tiene que ser lo suficiente como para coger un cubo)
*/
void apinza() {
  while (gradosPinza > GMIN) {
    gradosPinza = gradosPinza - 1;
    pinza.write(gradosPinza);
  }
}

/*
   Realiza una pausa de los milisegundos pasados por parametro.
*/
void wait(long milisegundos) {
  delay(milisegundos);
}

/*
   Metodo que parte un string de coordenadas con el formato X-tiempo
   leido por el serial por el separador ";".

*/
void generarArrayInstrucciones() {
  int i = 0;
  String linea = "";
  do {
    linea = Serial.readStringUntil(';');
    Serial.println(linea);
    instrucciones[i] = linea;
    i++;
  } while (linea != "" && i < 24);
}

/*
   Metodo que coloca el carro en la posicion inicial
   y devuelve el tiempo que tarda en hacer el recorrido en el eje X.
*/
long calibrarEjeX() {
  moverCarroFinEjeX();
  tiempoInicioEjeX = millis();
  moverCarroInicioEjeX();
  tiempoFinEjeX = millis();
  return tiempoFinEjeX - tiempoInicioEjeX;
}

/*
   Metodo que coloca el carro en la posicion inicial
   y devuelve el tiempo que tarda en hacer el recorrido en el eje X.
*/
long calibrarEjeY() {
  moverCarroFinEjeY();
  tiempoInicioEjeY = millis();
  moverCarroInicioEjeY();
  tiempoFinEjeY = millis();
  return tiempoFinEjeY - tiempoInicioEjeY;
}

/*
   Metodo que mueve el carro a la parte mas alta de la cinta.
*/
void moverCarroFinEjeY() {
  servoY.write(GIRO_HORARIO);
  while (digitalRead(botonTopeArriba) == HIGH) {
    topeArriba = false;
  }
  topeArriba = true;
}

/*
   Metodoque situa el carro en la posicion mas baja de la cinta.
*/
void moverCarroInicioEjeY() {
  servoY.write(GIRO_ANTI_HORARIO);
  while (digitalRead(botonTopeAbajo) == HIGH) {
    topeAbajo = false;
  }
  topeAbajo = true;
  servoY.write(PARADO);
}

/*
   Metodo que mueve el carro al final del eje x.
*/
void moverCarroFinEjeX() {
  servoX.write(GIRO_HORARIO);
  while (digitalRead(botonTopeIzquierda) == HIGH) {
    topeDerecha = false;
  }
  topeDerecha = true;
}

/*
   Metodo que situa el carro en la posicion inicial del eje x.
*/
void moverCarroInicioEjeX() {
  servoX.write(GIRO_ANTI_HORARIO);
  while (digitalRead(botonTopeDerecha) == HIGH) {
    topeIzquierda = false;
  }
  topeIzquierda = true;
  servoX.write(PARADO);
}
