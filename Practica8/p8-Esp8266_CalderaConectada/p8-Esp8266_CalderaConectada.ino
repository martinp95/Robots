#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define DHTPIN            2         // Pin which is connected to the DHT sensor.
#define DHTTYPE           DHT22     // DHT 22 (AM2302)

const char* ssid     = "Thom_D0050390";
const char* password = "8fb706cd395db3765bec52f733";
IPAddress ip(192, 168, 0, 100);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
ESP8266WebServer server(ip, 80);
const char* url_nevera_abierta = "https://maker.ifttt.com/trigger/nevera_abierta/with/key/vhPO_y3AYrcpjkBRLwuS-";
const char* url_nevera_bateriaBaja = "https://maker.ifttt.com/trigger/nevera_bateriaBaja/with/key/vhPO_y3AYrcpjkBRLwuS-";
const char* host = "maker.ifttt.com";

boolean ledEncendido = false;
const int led = 0;

/* SENSOR */
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;
float humedad;
float temperatura;
float temperaturaAlarma = 15;
float humedadAlarma = 90;
boolean aviso = false;
unsigned long tiempoSiguienteAviso = 0;

void leerSensor() {
  // Delay between measurements.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println("Error reading temperature!");
  }
  else {
    Serial.print("Temperature: ");
    temperatura = event.temperature;
    Serial.print(temperatura);
    Serial.println(" *C");
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println("Error reading humidity!");
  }
  else {
    Serial.print("Humidity: ");
    humedad = event.relative_humidity;
    Serial.print(humedad);
    Serial.println("%");
  }

  //delay(2500);
}




void handleLed() {
  if (!ledEncendido) {
    digitalWrite(led, HIGH);
    ledEncendido = true;
    Serial.println("LED ENCENDIDO");
    server.send(200, "text/plain", "LED ENCENDIDO");
    return;
  }

  if (ledEncendido) {
    digitalWrite(led, LOW);
    ledEncendido = false;
    Serial.println("LED APAGADO");
    server.send(200, "text/plain", "LED APAGADO");
  }

}


void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!");
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  Serial.print(message);
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;

  Serial.begin(9600);

  arrancarServer();

}

void loop(void) {
  leerSensor();
  
 /* if (temperatura < temperaturaAlarma) {
    enviarAlarmaTemperatura();
  } */
  
  if (humedad > humedadAlarma && (millis() >= tiempoSiguienteAviso  || tiempoSiguienteAviso == 0)) {
    enviarAlarmaTemperatura();
    //aviso = true;
    tiempoSiguienteAviso = millis() + 20000;
    //tiempoSiguienteAviso = millis() + 18000000;  //5mins
  } 

  server.handleClient();
}

void asdas() {
 
   String resp = "AVISO EUITIO -->Humedad: " + String(humedad) + "@";
 
}


void enviarAlarmaTemperatura(){
  
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  ////
  Serial.print("connecting to ");
  Serial.print(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  ////
  Serial.print("connecting to ");
  Serial.print(host);

 
  Serial.print("Requesting URL: ");
  Serial.println(url_nevera_abierta);
  
  // This will send the request to the server
  client.print(String("GET ") + url_nevera_abierta + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println();
  Serial.println("closing connection");
  Serial.println();
}

void arrancarServer(){
    WiFi.config(ip, gateway, subnet);
    WiFi.begin(ssid, password);
    Serial.println("");
  
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  
    if (MDNS.begin("esp8266")) {
      Serial.println("MDNS responder started");
    }
  
    server.on("/", handleRoot);
    
    //  /setTemperatura?temperatura=20
    server.on("/setTemperatura", handleSetTemperatura );
    
    //server.on("/sensor", handleSensor );
    server.onNotFound(handleNotFound);
  
    server.begin();
    Serial.println("HTTP server started");
    delay(1000);
  }

  void handleSetTemperatura(){
      for (uint8_t i = 0; i < server.args(); i++) {
        //message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
        if(server.argName(i) == "temperatura"){
         int newTemp = server.arg(i).toInt(); //float
         //temperaturaAlarma = newTemp;
         humedadAlarma = newTemp;
         Serial.println("TEMPERATURA DE ALARMA ACTUALIZADA: "+String(newTemp));
         server.send(200, "text/plain", "TEMPERATURA DE ALARMA ACTUALIZADA: "+String(newTemp));
         
        }
      }
      server.send(404, "text/plain", " PARA ACTUALIZAR TEMPERATURA USAR  \"<ip>/setTemperatura?temperatura=20\" ");
  }
