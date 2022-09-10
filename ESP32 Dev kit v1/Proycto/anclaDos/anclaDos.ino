#include <WiFi.h>
#include "ESPAsyncWebServer.h" //Para peticiones HTTP


const char* ssid = "monitoreoBovino";
const char* password = "monitoreoBovino123";

IPAddress ip(192,168,1,20);     
IPAddress gateway(192, 168, 1, 1);   
IPAddress subnet(255,255,255,0);   


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


String hellowWorld() {
  return String("Mensaje enviado desde nodo 2");
}


void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  Serial.println("Conectando");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());


    server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", hellowWorld().c_str());
  });

  
  server.begin();
}

void loop() {

}
