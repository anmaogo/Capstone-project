#include "WiFi.h"
#include "ESPAsyncWebServer.h" //Para peticiones HTTP



// CREDENCIALES
const char* ssid = "monitoreoBovino";
const char* password = "monitoreoBovino123";


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String hellowWorld() {
  return String("Hellow world, monitoreo Bovino!");
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println();
  
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", hellowWorld().c_str());
  });

  
  server.begin();
}
 
void loop(){
  
}
