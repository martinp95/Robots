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

boolean ledEncendido = false;
const int led = 0;
/* SENSOR */
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;
float humedad;
float temperatura;

boolean aviso = false;

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


void handleSensor() {
  String resp = "";
  if (aviso) {
    resp = "AVISO EUITIO -->Humedad: " + String(humedad) + "@";
  }
  resp += "Temp: " + String(temperatura) + "   Humedad: " + String(humedad) +
          " Led: " + ( ledEncendido == true ? "ENCENDIDO" : "apagado");

  server.send(200, "text/plain", resp );
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
  server.on("/led", handleLed );
  server.on("/sensor", handleSensor );
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  delay(1000);

}

void loop(void) {
  leerSensor();
  if (humedad > 90.00) {
    aviso = true;
  } else if (humedad < 70.00 && aviso ) {
    aviso = false;
  }

  server.handleClient();
}
