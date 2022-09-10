#include "WiFi.h"
#include <HTTPClient.h>


// CREDENCIALES
const char* ssid = "monitoreoBovino";
const char* password = "monitoreoBovino123";

//NODOS ANCLAS
const char* serverNodoUno = "http://192.168.1.10/data";
const char* serverNodoDos = "http://192.168.1.20/data";

IPAddress ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);


unsigned long previousMillis = 0;
const long interval = 5000; 
String anclaUno, anclaDos;


String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "--"; 
  
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


}
void loop() {
  unsigned long currentMillis = millis();
  
  if(currentMillis - previousMillis >= interval) {
    anclaUno = httpGETRequest(serverNodoUno);
    anclaDos = httpGETRequest(serverNodoDos);
    
    Serial.println(anclaUno);
    Serial.println(anclaDos);
    // save the last HTTP GET Request
    previousMillis = currentMillis;
    // Check WiFi connection status

  }
}

