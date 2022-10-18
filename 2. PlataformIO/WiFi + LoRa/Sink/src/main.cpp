#include <Arduino.h>

#include "WiFi.h"
#include <HTTPClient.h>

#include <SPI.h>
#include <LoRa.h>

#define led 2  

static constexpr float reference_power  = -23.703; //rssi reffrence 
static constexpr float distance_factor = 3.286;

static float get_distance(const int8_t rssi)
{ return pow(10, (reference_power - rssi)/(10*distance_factor)); }



// CREDENCIALES
const char* ssid = "monitoreoBovino";
const char* password = "monitoreoBovino123";

//Your IP address or domain name with URL path
const char* serverNodoUno = "http://192.168.1.10/data";
const char* serverNodoDos = "http://192.168.1.20/data";

IPAddress ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);


unsigned long previousMillis = 0;
const long interval = 5000; 
String mensaje;
String anclaUno, anclaDos;


String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}";  
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}



void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println();
  
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(ip, gateway, subnet);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  while (!Serial);

  Serial.println("LoRa Receiver");

   LoRa.setPins(17,16,22);                       // NSS, RESET y DIO0.

  while (!Serial);
  Serial.println("LoRa Receiver");
  if (!LoRa.begin(433E6)) { // or 915E6
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  pinMode(led,OUTPUT);


}
void loop() {
  unsigned long currentMillis = millis();
  int packetSize = LoRa.parsePacket();
  
  if(currentMillis - previousMillis >= interval) {
    anclaUno = httpGETRequest(serverNodoUno);
    anclaDos = httpGETRequest(serverNodoDos);
    Serial.println(mensaje);

    if (packetSize) {
    // received a packet
    Serial.print("Received packet '");


    digitalWrite(led,HIGH);                     // Parpadear LED al recibir el mensaje.
    delay(100);
    digitalWrite(led,LOW);

    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());

    //Serial.printf("La distancia es: %f \n", get_distance(LoRa.packetRssi()));   
  }

    //MENSAJES EN JSON
    

    // save the last HTTP GET Request
    previousMillis = currentMillis;
    // Check WiFi connection status

  }
}

