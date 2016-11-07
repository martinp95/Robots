//Pines de salida de las señales a los leds.
int ledVerde = 5;
int ledRojo = 4;

//Pines de entrada de la señal de los botones.
int botonVerde = 7;
int botonRojo = 8;

//Definimos constantes para los colores que coinciden con las de asignadas a los pines de los leds.
const int VERDE = 5;
const int ROJO = 4;

//Controlar estado del boton
//0 : boton sin pulsar
//1 : boton pulsado
int estadoBoton = 0;

//nivel del juego
int nivel;

const int SIZE = 12;
//Array de colores
int colores[SIZE];

//Nivel maximo
const int NIVEL_MAXIMO = 10;

//Nos indica si tenemos o no que mostrar los colores.
bool inicioNivel;

//Numero de leds a encender
int numeroLeds;

//Posicion a comprobar.
int posicionComprobar;


void setup() {
  Serial.begin(9600);
  //Ponemos los pines de los leds como salidas.
  pinMode(ledVerde, OUTPUT);
  pinMode(ledRojo, OUTPUT);

  //Ponemos los pines de los botones como entradas
  pinMode(botonVerde, INPUT);
  pinMode(botonRojo, INPUT);

  //Relleno el array con la secuencia de colores del juego, va de 4 a 5 por hacerlo coincidir con el pin de los leds.
  generarSecuencia();

  //Ponemos el nivel a 1.
  nivel = 1;

  //Penemos el numero de leds acorde al nivel 1.
  numeroLeds = 3;

  //Ponemos que hay que enseñar los colores.
  inicioNivel = true;

  //Al iniciar el juego siempre se empezara comprobando la primera posicion.
  posicionComprobar = 0;
}

void loop() {

  if (nivel <= NIVEL_MAXIMO) {
    //Comprobamos si debemos o no encender los leds
    if (inicioNivel) {
      //encendemos los leds
      for (int i = 0; i < numeroLeds; i++) {
        int colorMostrar = colores[i];
        switch (colorMostrar) {
          case VERDE:
            //Si es 5 encendemos el led verde durante un segundo.
            digitalWrite(ledVerde, HIGH);
            delay(1000);
            digitalWrite(ledVerde, LOW);
            delay(1000);
            break;
          case ROJO:
            // Si es 4 encendemos el led rojo durante un segundo.
            digitalWrite(ledRojo, HIGH);
            delay(1000);
            digitalWrite(ledRojo, LOW);
            delay(1000);
            break;
        }
      }
      inicioNivel = false;
    } else {
      //Espereamos una pulsacion
      int lecturaVerde = digitalRead(botonVerde);
      int lecturaRojo = digitalRead(botonRojo);

      //Compruebo que no se mantiene el boton pulsado
      if (lecturaVerde == HIGH && estadoBoton == 0) {
        Serial.print("Pulsacion Verde\n");
        //Si han pulsado el boton verde y estaba sin pulsar
        estadoBoton = 1;
        //Llamada al metodo para comprobar que se ha pulsado el color correcto.
        if (comprobar(VERDE, posicionComprobar)) {
          Serial.print("Pulsacion Verde correcta\n");
          //Si la pulsacion ha sido acertada incremento el indice que se deba de intentar comprobar.
          posicionComprobar++;
          //Comprar si se ha superado el nivel.
          if (nivelSuperado(posicionComprobar - 1, numeroLeds)) {
            //Aumentar el nivel simpre que no exceda el maximo.
            Serial.print("Nivel ");
            Serial.print(nivel);
            Serial.print(" superado\n");
            aumentarNivel();
          }
          
        } else { //Pulsacion fallada, reiniciar juego.
          Serial.print("Pulsacion Verde incorrecta\n");
          reiniciarJuego();
          Serial.print("Vuelva a empezar \nNivel ");
          Serial.print(nivel);
        }
      } else if (lecturaRojo == HIGH && estadoBoton == 0) {
        Serial.print("Pulsacion Rojo\n");
        //Si han pulsado el boton rojo y estaba sin pulsar
        estadoBoton = 1;
        //Llamada al metodo para comprobar que se ha pulsado el color correcto.
        if (comprobar(ROJO, posicionComprobar)) {
          Serial.print("Pulsacion Rojo correcta\n");
          //Si la pulsacion ha sido acertada incremento el indice que se deba de intentar comprobar.
          posicionComprobar++;
          //Comprar si se ha superado el nivel.
          if (nivelSuperado(posicionComprobar - 1, numeroLeds)) {
            Serial.print("Nivel ");
            Serial.print(nivel);
            Serial.print(" superado\n");
            //Aumentar el nivel simepre que no exceda el maximo.
            aumentarNivel();
          }
          
        } else { //Pulsacion fallada, reiniciar juego.
          Serial.print("Pulsacion Rojo incorrecta\n");
          reiniciarJuego();
          Serial.print("Vuelva a empezar \nNivel ");
          Serial.print(nivel);
        }
      } else if (lecturaRojo == LOW && lecturaVerde == LOW) {
        //Si ninguno de los dos botones esta pulsado, espero por una nueva pulsacion.
        estadoBoton = 0;
      }
    }
  } else {
    Serial.print("Enhorabuena ha ganado!!!");
  }
}
/*Metodo para comprobar que el color pulsado es correcto conforme a la secuencia.
   Recibe como parametros el boton que se pulsa y la posicion del array que se va a comprobar.
*/
bool comprobar(int boton, int posicion) {
  bool aux = false;
  switch (boton) {
    case VERDE:
      if (VERDE == colores[posicion])
        aux = true;
      else
        aux = false;
      break;
    case ROJO:
      if (ROJO == colores[posicion])
        aux = true;
      else
        aux = false;
      break;
  }
  return aux;
}

/*
   Metodo mediante el que se comprueba si se ha superado o no el nivel.
*/
bool nivelSuperado(int posicion, int numeroLeds) {
  if (posicion == numeroLeds - 1) {
    return true;
  } else {
    return false;
  }
}

/*
   Metodo para aumentar el nivel si el anterior ha sido superado.
*/
void aumentarNivel() {
  posicionComprobar = 0;
  inicioNivel = true;
  numeroLeds++;
  estadoBoton = 0;
  nivel++;
}

/*
   Metodo para reinicar el juego una vez que se falla o se gana.
*/
void reiniciarJuego() {
  nivel = 1;
  numeroLeds = 3;
  estadoBoton = 0;
  posicionComprobar = 0;
  inicioNivel = true;
  generarSecuencia();
}

/*
   Relleno el array con la secuencia de colores del juego, va de 4 a 5 por hacerlo coincidir con el pin de los leds.
*/
void generarSecuencia() {
  for (int i = 0; i < SIZE; i++) {
    colores[i] = random(4, 6);
  }
}
