#include <WiFi.h>
#include <WebServer.h>
 
// Creamos nuestra propia red -> SSID & Password
const char* ssid = "monitoreoBovino";  
const char* password = "monitoreoBovino12"; // opcional softAP()
 

// Definimos la IP local
IPAddress ip(192,168,4,22);         //(192, 168, 1, 1)
IPAddress gateway(192,168,4,9);     //(192, 168, 1, 1)
IPAddress subnet(255,255,255,0);    //(255, 255, 255, 0)

WebServer server(80);  // puerto por defecto 80



void setup() {
  Serial.begin(115200);
 
  // Creamos el punto de acceso
  WiFi.softAP(ssid, password); // Tiene mas parametros opcionales
  WiFi.softAPConfig(ip, gateway, subnet);
  //IPAddress ip = WiFi.softAPIP();
//  Serial.println(ip);
 
 
  Serial.print("Nombre de mi red esp32: ");
  Serial.println(ssid);
 
  server.on("/", handleConnectionRoot); //Responder ante cualquier evento de petición HTTP
 
  server.begin();
  Serial.println("Servidor HTTP iniciado");
  delay(150);
}
 
void loop() {
  server.handleClient();
}
 
// Nuestra respuesta en html
String answer = "<!DOCTYPE html>\
<html>\
<body>\
<h1>Hola desde ESP32 - Modo Punto de Acceso (AP)</h1>\
</body>\
</html>";

 
// Responder a la url raíz (root /)
void handleConnectionRoot() {
  server.send(200, "text/html", answer);
}
