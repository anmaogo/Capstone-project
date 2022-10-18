#include <WiFi.h>
#include "ESPAsyncWebServer.h" //Para peticiones HTTP
#include <SPI.h>
#include <LoRa.h>



const char* ssid = "monitoreoBovino";
const char* password = "monitoreoBovino123";

IPAddress ip(192,168,1,10);     
IPAddress gateway(192, 168, 1, 1);   
IPAddress subnet(255,255,255,0); 


#define led 2                                // LED utilizado para notificar la emisión de un mensaje.
int counter = 0;


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "application/json", "{\"message\":\"Not found\"}");

}

String hellowWorld() {
  return String("Mensaje enviado desde nodo 1");
}

// mensaje
String id = "AnclaUno";
String data;

//--------------------------------------------------------------------------------------------------
// Star Web Services
//----------------------------------------------------------------------------------------------

void startWebServer() {
 server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", data.c_str());
  });
  server.begin();
}
///////////////////////////////////////////////////

//--------------------------------------------------------------------------------------------------
// Set Up Wifi 
//--------------------------------------------------------------------------------------------------
void setupWIFI()
{
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
}


static constexpr float reference_power  = -50; //rssi reffrence 
static constexpr float distance_factor = 3.5;


void setup() {
  Serial.begin(115200);
  setupWIFI();
  while (!Serial);
  

  Serial.println("LoRa Sender");

   LoRa.setPins(17,16,22);                       // NSS, RESET y DIO0.


  while (!Serial);  
  Serial.println("LoRa Sender");
  if (!LoRa.begin(433E6)) { // or 915E6, the MHz speed of your module
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  pinMode(led,OUTPUT);   

}

void loop() {

  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  digitalWrite(led,HIGH);                   // Parpadear LED al enviar el mensaje.
  delay(100);
  digitalWrite(led,LOW);

  counter++;

  delay(1000);

}