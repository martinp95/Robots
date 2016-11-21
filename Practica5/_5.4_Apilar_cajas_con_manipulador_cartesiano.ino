#include <Servo.h>
#define botonTopeDerecha 5
#define botonTopeIzquierda 6
#define botonTopeArriba 7
#define botonTopeAbajo 8
#define pinServoX 9
#define pinServoY 10
#define pinPinza 11
// Grados pinza, MAX y MIN admitido
#define GMAX 108
#define GMIN 5

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
int posicionC1X = 24;
int posicionC2X = 12;
int posicionC3X = 0;
bool variable = false;


void setup() {
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


/*
   Abre la pinza los grados son conocidos por el robot
*/
void apinza() {
 
  while (gradosPinza > GMIN) {
    gradosPinza = gradosPinza - 1;
    pinza.write(gradosPinza);
  }
  wait(200);
}
/*
   Cierra la pinza, los grados son conocidos por el robot(tiene que ser lo suficiente como para coger un cubo)
*/
void cpinza() {
  while (gradosPinza < GMAX) {
    gradosPinza += 1;
    pinza.write(gradosPinza);
  }
  wait(200);
}






void loop() {
  // put your main code here, to run repeatedly:
  if(!variable){
  generarInstrucciones();
  cargarInstrucciones();
  variable = true;
  }
}
void generarInstrucciones() {
  //llegar a la primera caja
  instrucciones[0] = "MOVPY(24,2)";
  instrucciones[1] = "MOVPY(24,0)";
  //recoger caja
  instrucciones[2] = "CPINZA";
  //llevar caja a destino
  instrucciones[3] = "MOVPY(0,3)";
  instrucciones[4] = "APINZA";
  //////////  //////////  //////////
  instrucciones[5] = "MOVPY(12,7)";
  instrucciones[6] = "MOVPY(12,0)";
  instrucciones[7] = "CPINZA";
   
  instrucciones[8] = "MOVPY(0,6)";
  instrucciones[9] = "APINZA";
  instrucciones[10] = "MOVPY(0,15)";

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
   Realiza una pausa de los milisegundos pasados por parametro.
*/
void wait(long milisegundos) {
  delay(milisegundos);
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