#include <Servo.h>
#define botonTopeDerecha 6// Boton final derecha
#define botonTopeIzquierda 5// Boton final izquierda

Servo servo;

//Constantes movimiento servo motor.
const int DERECHA = 180;
const int IZQUIERDA = 0;
const int PARADO = 90;

//Variable para guardar el tiempo que tarda el carro en ir de un tope a otro.
long tiempoTotal;
long tiempoInicio;
long tiempoFin;

//Variable para saber si debe o no calibrar el carro.
//calibrado = true el sistema esta calibrado
//calibrado = false el sistema ha de calibrarse

//Variables control
bool topeDerecha;
bool topeIzquierda;

//Array de cordenadas
String coordenadas[24];
int coordenadaActual = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servo.attach(9);
  pinMode(botonTopeDerecha, INPUT);
  pinMode(botonTopeIzquierda, INPUT);

  //topeDerecha = false;
  tiempoTotal = calibrar();
}

void loop() {
  //Esperamos las cordenadas para mover el carro.
  //Leer coordenadas de serial.
  if (Serial.available() > 0) {
    //Generar Array coordenadas
    generarArrayCoordenadas();
    Serial.println("***********");
    //Iniciar movimiento carro a coordenadas.
    moverCarroCoordenadas();
    Serial.println("***********");
  }
}

/*
   Metodo que parte un String co elformato X-tiempo en dos
   variables coordenadaX y tiempoEspera en esa coordenada.
*/
void moverCarroCoordenadas() {
  int i = 0;
  while (coordenadas[i] != "") {
    int coordenadaX = coordenadas[i].substring(0, coordenadas[i].indexOf("-")).toInt() ;
    long tiempoEspera = coordenadas[i].substring(coordenadas[i].indexOf("-") + 1, coordenadas[i].length()).toInt();
    Serial.print(coordenadaX);
    Serial.print("-");
    Serial.println(tiempoEspera);

    //mover el carro a la posicion y esperar.
    mover(coordenadaX, tiempoEspera);
    i++;
  }
}

/*
   Metodo que mueve el carro a la posicion que se le pasa como parametro
   y espera en esa posicion el tiempo que se le pasa como parametro.
*/
int mover(int coordenadaX, long tiempoEspera) {
  if (coordenadaX > coordenadaActual) {
    servo.write(DERECHA);
    delay((tiempoTotal / 24 * (coordenadaX - coordenadaActual)));
    servo.write(PARADO);
    delay(tiempoEspera);
  } else if (coordenadaX < coordenadaActual) {
    servo.write(IZQUIERDA);
    delay(tiempoTotal / 24 * abs(coordenadaX - coordenadaActual));
    servo.write(PARADO);
    delay(tiempoEspera);
  }
  coordenadaActual = coordenadaX;

}
/*
   Metodo que parte un string de coordenadas con el formato X-tiempo
   leido por el serial por el separador ",".

*/
void generarArrayCoordenadas() {
  int i = 0;
  String linea = "";
  do {
    linea = Serial.readStringUntil(',');
    Serial.println(linea);
    coordenadas[i] = linea;
    i++;
  } while (linea != "" && i < 24);
}

/*
   Metodo que coloca el carro en la posicion inicial
   y devuelve el tiempo que tarda en hacer el recorrido.
*/
long calibrar() {
  moverCarroFin();
  tiempoInicio = millis();
  moverCarroInicio();
  tiempoFin = millis();
  return tiempoFin - tiempoInicio;
}

/*
   Metodo que mueve el carro al final de la cinta.
*/
void moverCarroFin() {
  servo.write(DERECHA);
  while (digitalRead(botonTopeDerecha) == HIGH) {
    topeDerecha = false;
  }
  topeDerecha = true;
}

/*
   Metodoque situa el carro en la posicion inicial de la cinta.
*/
void moverCarroInicio() {
  servo.write(IZQUIERDA);
  while (digitalRead(botonTopeIzquierda) == HIGH) {
    topeIzquierda = false;
  }
  topeIzquierda = true;
  servo.write(PARADO);
}