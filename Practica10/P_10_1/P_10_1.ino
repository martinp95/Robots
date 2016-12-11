#include <Servo.h>
#include <LiquidCrystal_I2C.h> // Descargar la lib liquidcrystal
int LINEA = 1;
int NO_LINEA = 0;

Servo servoI;
Servo servoD;
boolean modoPausa = 1;

// Pines: sensores de izquierda a derecha 1 2 3 4 
int pinSensor1 = 10;
int pinSensor2 = 2;
int pinSensor3 = 3;
int pinSensor4 = 11;

// Valores
int Sensor1 = -1;
int Sensor2 = -1;
int Sensor3 = -1;
int Sensor4 = -1; 
boolean meta = false;

char camino[30];
int indiceCamino = 0;

// el primer código puede ser 0x27 o 0x3F
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  
// Set the LCD I2C address

int sensoresActivos;

void setup()
{
  Serial.begin(9600);
  servoI.attach(8);
  servoD.attach(9);
  lcd.begin(20,4); // 20 caracteres 4 lineas     
  
  pinMode( pinSensor1 , INPUT);
  pinMode( pinSensor2 , INPUT);
  pinMode( pinSensor3 , INPUT);
  pinMode( pinSensor4 , INPUT);

  pinMode( 13 , OUTPUT);
  digitalWrite(13,LOW);
  // Pausa inicial de 10 segundos
  delay(1000);

  // 3 parpadeos al inicio 
  for(int i = 0; i< 3; i++){
    lcd.backlight();
    delay(250);
  lcd.noBacklight();
    delay(250);
  }
  // Poner luz
  lcd.backlight(); 

  
}

void mostrarCaminoPantalla(){  // Colocar un texto
  // La posición del cursor comienza en  0  
  lcd.setCursor(4,0);
  lcd.print("CAMINO: ");
  for(int i = 0 ; i< indiceCamino ; i++){
      lcd.print(camino[i]);
      lcd.print('-');
  }
}



void girarDerecha(){     
   // girar hacia la derecha 
   servoD.write(0);
   servoI.write(0);  
   delay(400); // Pequeño delay para que le de tiempo a salir de su carril actual
   // Si no puede que este ya en un carril y detectaria linea 

   
   boolean carrilEncontrado = false;
   while(!carrilEncontrado) {
        Sensor2 = digitalRead(pinSensor2);
        Sensor3 = digitalRead(pinSensor3);
        if( Sensor2 == LINEA && Sensor3 == LINEA ){
          carrilEncontrado = true;
        }
    }
   // Parar
   servoD.write(90);
   servoI.write(90); 
   camino[indiceCamino] = 'D';
   indiceCamino++;
}

void girarIzquierda(){     
   // girar hacia la izquierda 
   servoD.write(180);
   servoI.write(180);  
   delay(400); // Pequeño delay para que le de tiempo a salir de su carril actual
   // Si no puede que este ya en un carril y detectaria linea 

   
   boolean carrilEncontrado = false;
   while(!carrilEncontrado) {
        Sensor2 = digitalRead(pinSensor2);
        Sensor3 = digitalRead(pinSensor3);
        if( Sensor2 == LINEA && Sensor3 == LINEA ){
          carrilEncontrado = true;
        }
    }
      
   // Parar
   servoD.write(90);
   servoI.write(90); 
   camino[indiceCamino] = 'I';
   indiceCamino++;
}

void avanzar(){
    servoD.write(180); 
    servoI.write(0);
    delay(400);
   camino[indiceCamino] = 'A';
   indiceCamino++;
}


void darMediaVuelta(){
    // Gira sobre si mismo hasta encontrar el carril en el centro ( _  X X  _ )
    // Cuando lo encuentra para
    
    // Girar sobre el eje
    servoD.write(180);
    servoI.write(180);  
    
    boolean carrilEncontrado = false;
    while(!carrilEncontrado) {
        Sensor2 = digitalRead(pinSensor2);
        Sensor3 = digitalRead(pinSensor3);

        if( Sensor2 == LINEA || Sensor3 == LINEA ){
          carrilEncontrado = true;
        }
    }
    
    // Parar
    servoD.write(90);
    servoI.write(90); 
    camino[indiceCamino] = 'R';

    indiceCamino++;
}


int sensores[] = {-1,-1,-1,-1};

void actualizarTodosLosSensores(){   
  sensores[0] = digitalRead(pinSensor1);
  sensores[1]  = digitalRead(pinSensor2);
  sensores[2]  = digitalRead(pinSensor3);z
  sensores[3]  = digitalRead(pinSensor4);
  
  for(int i=0; i < 4; i++){
    delay(20);
    
    if (digitalRead(pinSensor1) == LINEA )
      sensores[0] = LINEA;
    if (digitalRead(pinSensor2) == LINEA )
      sensores[1] = LINEA;
    if (digitalRead(pinSensor3) == LINEA )
      sensores[2] = LINEA;
    if (digitalRead(pinSensor4) == LINEA )
      sensores[3] = LINEA;
  }
  
    Sensor1 = sensores[0];
    Sensor2 = sensores[1];
    Sensor3 = sensores[2];
    Sensor4 = sensores[3];
}


void reducirRuta(){

  int indice = 0;
  int indiceMax = 30;

  lcd.setCursor(4,0);
  lcd.print("REDUCIENDO CAMINO: ");
  
  for(int j=0; j< indiceMax; j++){
    if(camino[indice] == 'A' && camino[indice+1] == 'R' && camino[indice+2] == 'I'){
      camino[indice]='D';
      for(int i = indice+1 ; i<indiceMax-2; i++){
        camino[i]=camino[i+2];
      }
      
    }else if(camino[indice] == 'I' && camino[indice+1] == 'R' && camino[indice+2] == 'I'){
      camino[indice]='A';
      for(int i = indice+1 ; i<indiceMax-2; i++){
        camino[i]=camino[i+2];
      }
    }else if(camino[indice] == 'I' && camino[indice+1] == 'R' && camino[indice+2] == 'A'){
      camino[indice]='D';
      for(int i = indice+1 ; i<indiceMax-2; i++){
            camino[i]=camino[i+2];
          }
    }else if(camino[indice] == 'D' && camino[indice+1] == 'R' && camino[indice+2] == 'A'){
      camino[indice]='R';
      for(int i = indice+1 ; i<indiceMax-2; i++){
            camino[i]=camino[i+2];
          }
    }else if(camino[indice] == 'D' && camino[indice+1] == 'R' && camino[indice+2] == 'I'){
      camino[indice]='D';
      for(int i = indice+1 ; i<indiceMax-2; i++){
            camino[i]=camino[i+2];
          }
    }else if(camino[indice] == 'A' && camino[indice+1] == 'R' && camino[indice+2] == 'A'){
      camino[indice]='D';
      for(int i = indice+1 ; i<indiceMax-2; i++){
            camino[i]=camino[i+2];
          }
    }
  }
  mostrarCaminoPantalla();

  
}


void loop()
{


  mostrarCaminoPantalla();
  
  actualizarTodosLosSensores();
if(meta){
      servoD.write(90); 
      servoI.write(90);
      digitalWrite(13, HIGH);

      reducirRuta();
      
      return;
}


  if ( Sensor1 == LINEA && Sensor2 == LINEA && Sensor3 == LINEA && Sensor4 == LINEA ){
      // 3 Posibilidades:  Cucre izq-der || Cruce adelante-izq-der || Meta
      // cruce: X X X X 
      // Serial.println("Cruce izq-der || Cruce adelante-izq-der || Meta");
      
      // Avanzar un poco
      servoD.write(180); // alante
      servoI.write(0); // alante
      delay(300); 
      
      // Parar
      servoD.write(90); 
      servoI.write(90);  
      delay(500); 
      
      actualizarTodosLosSensores();
  
     if( Sensor1 == LINEA && Sensor2 == LINEA && Sensor3 == LINEA && Sensor4 == LINEA ){
        // Serial.println("Meta");
        // Delante del cruce: X X X X 
        // Meta
        meta = true;
        return;      
      } else if (Sensor1 == NO_LINEA && Sensor2 == NO_LINEA && Sensor3 == NO_LINEA && Sensor4 == NO_LINEA){
        // Delante del cruce: _ _ _ _
        // Cucre izq-der
        // Serial.println("Cucre izq-der");
    girarIzquierda(); 
        return;
  
      } else {
        // Delante del cruce: _ X X _ 
        // Cruce adelante-izq-der
        // Serial.println("Cucre adelante-izq-der");
    girarIzquierda(); 
        return;
      }

      
  } else if (Sensor1 == LINEA && Sensor2 == LINEA && Sensor3 == LINEA && Sensor4 == NO_LINEA  )  {
      // Curva de izquierdas || Curva a izquierdas y frente
      // X X X _ 
      //Serial.println("Curva de izquierdas || Curva a izquierdas y frente");
      
      // Avanzar un poco
      servoD.write(180); 
      servoI.write(0); 
      delay(300); 
  
      // Parar
      servoD.write(90); 
      servoI.write(90);  
      delay(2000); 
  
      // Actualizar sensores centrales
      actualizarTodosLosSensores();
  
      if( Sensor1 == LINEA || Sensor2 == LINEA || Sensor3 == LINEA  ){
        // Algo delante tiene linea
        // Cruce adelante-izq
        // X X X _ 
    avanzar();
        return;
        
      } else {
        // Curva izq
    girarIzquierda();
        return;
      }
   
  } else if (Sensor1 == NO_LINEA && Sensor2 == NO_LINEA && Sensor3 == NO_LINEA && Sensor4 == NO_LINEA){
      // Camino sin salida media vuelta
      // _ _ _ _ 
      // Serial.println("Sin salida");
      darMediaVuelta();
      return;
  } else if  (Sensor1 == NO_LINEA && Sensor2 == LINEA && Sensor3 == LINEA && Sensor4 == LINEA) {
        // Curva de derechas || Curva a derechas y frente
        // _ X X X
       // Serial.println("Curva de derechas || Curva a derechas y frente");
        
        // Avanzar un poco
        servoD.write(180); 
        servoI.write(0); 
        delay(300); 
    
        // Parar
        servoD.write(90); 
        servoI.write(90);  
        delay(2000); 
    
        // Actualizar sensores
        actualizarTodosLosSensores();
    
        if(  Sensor2 == LINEA || Sensor3 == LINEA || Sensor4 == LINEA  ){
          // Algo de linea delante, es una curva a derechas y frente
          // _ X X X
      avanzar();
          return;
          
        } else {
          // Solo Curva a derechas
          // Serial.println("Curva der");
      girarDerecha();
          return;
        }
   
  } else if  (Sensor1 == NO_LINEA &&  Sensor2 == LINEA && Sensor3 == LINEA && Sensor4 == NO_LINEA  ) {
    // Seguir carril central hacia delante
    // _  X X  _ 
    // Serial.println("Seguir carril central hacia delante");
    servoD.write(180); 
    servoI.write(0);
    return;

  } else if ( Sensor2 == NO_LINEA && Sensor3 == LINEA ) {
    // Seguir carril central - Reajuste a la izquierda
    //  _ _ X _ 
    // Serial.println("Seguir carril central - Reajuste a la izquierda");
    servoD.write(90); 
    servoI.write(80);
    return;
 
  } else if (Sensor2 == LINEA && Sensor3 == NO_LINEA ) {
    // Seguir carril central - Reajuste a la derecha
    // _ X _ _ 
    // Serial.println("Seguir carril central - Reajuste a la derecha");
    servoD.write(100);
    servoI.write(90);
    return;
 // QUITAR
  }  else {
     //servoD.write(90);
     //servoI.write(90); 
  }
  
}


