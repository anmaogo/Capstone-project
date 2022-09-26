#include <WiFi.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include "ESPAsyncWebServer.h"

IPAddress ip(192,168,1,10);     
IPAddress gateway(192, 168, 1, 1);   
IPAddress subnet(255,255,255,0);  

const char* ssid = "monitoreoBovino";
const char* password = "monitoreoBovino123";

int scanTime = 2; //In seconds
BLEScan* pBLEScan;

AsyncWebServer server(80);

// mensaje
String id = "AnclaUno";
String data;

String mensaje() {
   
  return "Hola mundo";
}


//--------------------------------------------------------------------------------------------------
// Star Web Services
//----------------------------------------------------------------------------------------------

void startWebServer() {
 server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", data.c_str());
  });
  server.begin();
}

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

//--------------------------------------------------------------------------------------------------
// Task: BLE Scan 
//--------------------------------------------------------------------------------------------------

void bleScanResult(){
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(2000);  

  }



//--------------------------------------------------------------------------------------------------
// BLE SET UP
//--------------------------------------------------------------------------------------------------
void setupBLE()
{


class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      
      //Serial.printf("Advertised Device: %s \n", advertisedDevice.getAddress().toString().c_str());
      String anchorName;
      signed int rssi;
      if(advertisedDevice.getAddress().toString() == "30:c6:f7:2f:b1:de"){
        if (advertisedDevice.haveRSSI()){
          //Serial.printf("Rssi: %d \n", (int)advertisedDevice.getRSSI());          
          Serial.println("Se encontró nodo");      
          Serial.printf("Nodo: %s Rssi: %d \n", advertisedDevice.getName().c_str() , (int)advertisedDevice.getRSSI());

          char buffer [5];        
          anchorName = advertisedDevice.getName().c_str();
          rssi = (int)advertisedDevice.getRSSI();
          sprintf(buffer, "%d", rssi);
          data =  id + ";"+ anchorName + ";" + buffer;
           
        
          }
        //Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
      }


    }
};


  Serial.println("Scanning...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval 

  
}


//--------------------------------------------------------------------------------------------------
// Void Set up: Set up BLE & WiFi 
//--------------------------------------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);

  setupWIFI();    
  setupBLE();
}

//--------------------------------------------------------------------------------------------------
// Loop: Scan BLE
//--------------------------------------------------------------------------------------------------
void loop() {

  bleScanResult();

  if(WiFi.status() == WL_CONNECTED){
    startWebServer();

  }else{
    setupWIFI(); 
  }

  delay(30000);


}