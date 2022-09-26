#include "WiFi.h"
#include <HTTPClient.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

// CREDENCIALES
const char* ssid = "monitoreoBovino";
const char* password = "monitoreoBovino123";

//NODOS ANCLAS
const char* serverNodoUno = "http://192.168.1.10/data";
const char* serverNodoDos = "http://192.168.1.20/data";

IPAddress ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

int scanTime = 2; //In seconds
BLEScan* pBLEScan;


unsigned long previousMillis = 0;
const long interval = 5000; 
String anclaUno, anclaDos;


//////////////////////////////////////////////// BLE ////////////////////////////////////////////////////////////////

// ------------------------------------------------------------
// Ble scan result
// ------------------------------------------------------------


void bleScanResult(){
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(2000);  

  }

// ------------------------------------------------------------
// Ble set up
// ------------------------------------------------------------

void setUpBle()
{
  
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
       Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());     

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


//////////////////////////////////////////////// WI FI //////////////////////////////////////////////////////////////

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "--"; 
  
  if (httpResponseCode>0) {
    //Serial.print("HTTP Response code: ");
    //Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    //Serial.print("Error code: ");
    //Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}


// ------------------------------------------------------------
// Set Up WiFi
// ------------------------------------------------------------

void setUpWiFi()
{
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(ip, gateway, subnet);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

}

// ------------------------------------------------------------
// Void Set up
// ------------------------------------------------------------

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println();

  setUpWiFi();
  setUpBle();
  

}
void loop() {

  bleScanResult();
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

