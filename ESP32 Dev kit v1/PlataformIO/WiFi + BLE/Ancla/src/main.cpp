/*
   TRABAJO DE GRADO MAESTRÍA IOT
   NODO ANCLA

*/

#include <Arduino.h>

// Libraries space

#include <WiFi.h>
#include "ESPAsyncWebServer.h" //Para peticiones HTTP



int count1 = 0;
int count2 = 0;

// Tasks handle space

TaskHandle_t task1_handle = NULL; 

const char* ssid = "monitoreoBovino";
const char* password = "monitoreoBovino123";

IPAddress ip(192,168,1,10);     
IPAddress gateway(192, 168, 1, 1);   
IPAddress subnet(255,255,255,0);  


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


String hellowWorld() {
  return String("Mensaje enviado desde nodo 1");
}

// ----------------------------------------------------
//  Void 1: Web server initialization
// ----------------------------------------------------

void startWebServer() {
 server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", hellowWorld().c_str());
  });
  server.begin();
}

// ----------------------------------------------------
//  Void 2: Set Up WiFi Services
// ----------------------------------------------------

void setUpWiFiServices() {

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
  
  startWebServer();
  
}

// ----------------------------------------------------
//  Task 1: 
// ----------------------------------------------------

void task1(void * parameters){
  for(;;){
    Serial.print("Task 1 counter: ");
    Serial.println(count1++);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

      
  }
}

// ----------------------------------------------------
//  Task 1: 
// ----------------------------------------------------

void task2(void * parameters){
  for(;;){
    Serial.print("Task 2 counter: ");
    Serial.println(count2++);
    vTaskDelay(1000 / portTICK_PERIOD_MS);    
  }
}


void setup() {
  // put your setup code here, to run once:

  setUpWiFiServices();
  
  Serial.begin(115200);
  xTaskCreate(
    task1, //Fuction name
    "Task 1", //Task name
    1000, //Stack size
    NULL, //Task parameters
    1, //Task priority
    NULL //task handle
  );

  xTaskCreate(
    task2, //Fuction name
    "Task 2", //Task name
    1000, //Stack size
    NULL, //Task parameters
    1, //Task priority
    NULL //task handle
  );
}



void loop() {
  // put your main code here, to run repeatedly:

}