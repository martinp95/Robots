#include <Servo.h>
#define X_pin A0 // pin analógico para leer eje X
#define boton_pin 7 // pin digital para el botón
#define botonTopeDerecha 5// Boton final derecha
#define botonTopeIzquierda 6// Boton final izquierda

Servo servo;
const int PARADO = 90;
const int ALANTE = 0;
const int ATRAS = 180;

const int AUTOMATICO =1;
const int  MANUAL = 0;
//Variable que controla si el estado es automatico o manual
int estado;

//Controlar estado del boton
//0 : boton sin pulsar
//1 : boton pulsado
const int PULSADO = 1;
const int NO_PULSADO = 0;
//Variable que indica si el boton esta pulsado o no.
int estadoBoton;

//Variable que guarda la direccion del movimiento del brazo.
int direccionMovimiento;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servo.attach(9);
  estado = MANUAL;
  pinMode(boton_pin, INPUT_PULLUP);
  pinMode(botonTopeDerecha, INPUT);
  pinMode(botonTopeIzquierda, INPUT);
  estadoBoton = NO_PULSADO;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (estado == MANUAL) {
    int valorX = analogRead(X_pin);
    if (valorX > 412 && valorX < 612) {
      servo.write(PARADO);
      direccionMovimiento = PARADO;
    } else if (valorX < 412) {
      servo.write(ATRAS);
      direccionMovimiento = ATRAS;
    } else if (valorX > 612) {
      servo.write(ALANTE);
      direccionMovimiento = ALANTE;
    }
    
  } else if (estado == AUTOMATICO) {
    if (digitalRead(botonTopeIzquierda) == LOW) {
      servo.write(ATRAS);
      direccionMovimiento = ATRAS;
      
    } else if (digitalRead(botonTopeDerecha) == LOW) {
      servo.write(ALANTE);
      direccionMovimiento = ALANTE;

    } else if(direccionMovimiento == PARADO){
      servo.write(ALANTE);
      direccionMovimiento = ALANTE;
    }
  }
int lecturaBoton = digitalRead(boton_pin);
  if (estadoBoton == PULSADO &&  lecturaBoton == HIGH) {
      cambiarEstado();//Pasamos al modo automatico.
      estadoBoton = NO_PULSADO;
    } else if (estadoBoton == NO_PULSADO && lecturaBoton == LOW) {
      estadoBoton = PULSADO;
    }
  
}

void cambiarEstado() {
  if (estado == MANUAL) {
    estado = AUTOMATICO;
  } else {
    estado = MANUAL;
  }
}

