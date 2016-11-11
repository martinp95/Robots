#include <Servo.h> 

#define bumperDerecha 10
#define pin_Xservo 7
#define pin_X_Joy A1


const int PARADO = 90;
const int DERECHA = 0;
const int IZQUIERDA = 180;

Servo servoX;

int instrucciones[100];
long tiempos[100];
int estadoActual = PARADO;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(bumperDerecha, INPUT);
  servoX.attach(pin_Xservo);
  moverInicio();
}

void loop() {
  // put your main code here, to run repeatedly:
  char cadena = Serial.read();  
  moverInicio();
 Serial.println("Esperando instrucciones............");
  if(cadena == 'g'){
    Serial.println("GRABANDO");
    grabar();
  }
  else if(cadena == 'r')
    play();

}

void grabar(){
  int contadorAcciones = 0; 
  moverInicio();
  int valorX;
 
  char  cadena = Serial.read(); 
   long inicioAccion = millis();
  while(cadena != 'r'){
   
 
    valorX = analogRead(pin_X_Joy);
    cadena = Serial.read();
    if(valorX >600){
      if(estadoActual != DERECHA){     
         long finAccion = millis();    
         tiempos[contadorAcciones] = finAccion - inicioAccion;
         instrucciones[contadorAcciones] = estadoActual;
         estadoActual = DERECHA;
         servoX.write(estadoActual);
         contadorAcciones++;  
         inicioAccion = millis();
      }  
    } else if(valorX < 400) {
      if(estadoActual != IZQUIERDA){
         long finAccion = millis();
         tiempos[contadorAcciones] = finAccion - inicioAccion;
         instrucciones[contadorAcciones] = estadoActual;
         estadoActual = IZQUIERDA;
          servoX.write(estadoActual);
         contadorAcciones++;  
         inicioAccion = millis();
        }
    } else {
     if(estadoActual != PARADO){
         long finAccion = millis();
         tiempos[contadorAcciones] = finAccion - inicioAccion;
         instrucciones[contadorAcciones] = estadoActual;
         estadoActual = PARADO;
         servoX.write(estadoActual);
         contadorAcciones++;  
         inicioAccion = millis();
        }
    }
  }
}

void play(){
 
  for(int i = 0; i < 100; i++){
            
      Serial.print(instrucciones[i]);
      Serial.print("\t");
      Serial.println(tiempos[i]);

    servoX.write(instrucciones[i]);
   
    long comienzo = millis();
    long tiempoTranscurrido = 0;
     while(tiempoTranscurrido < tiempos[i]){     
      long fin = millis();
      tiempoTranscurrido = fin-comienzo;
     }
      
    }

}


void moverInicio(){
  servoX.write(DERECHA);
  int golpeDerecha = digitalRead(bumperDerecha);
  
  while(golpeDerecha == HIGH)
    golpeDerecha = digitalRead(bumperDerecha);
}
