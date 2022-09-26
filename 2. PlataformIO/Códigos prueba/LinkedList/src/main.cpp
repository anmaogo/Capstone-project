#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <LinkedList.h>

// Este es la información de un nodo

String nameDevice = "FirstNode";

struct item
{
  /* data */
  //String foundByNameDevice;
  String foundByNameDevice;
  String foundNameDevice;
  int rssi;
};

int scanTime = 1; //In seconds
BLEScan* pBLEScan;


LinkedList<item> *myLinkedList = new LinkedList<item>();
// Make sure you call delete(MyClass) when you remove!

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {

            //Serial.printf("Advertised Device: %s \n", advertisedDevice.getAddress().toString().c_str());
      
      signed int rssi;
      if(advertisedDevice.getAddress().toString() == "30:c6:f7:2f:b1:de"){
        if (advertisedDevice.haveRSSI()){
          //Serial.printf("Rssi: %d \n", (int)advertisedDevice.getRSSI());          
          //Serial.println("Se encontró nodo");      
          //Serial.printf("Nodo: %s Rssi: %d \n", advertisedDevice.getName().c_str() , (int)advertisedDevice.getRSSI());
          item item_temp{nameDevice,advertisedDevice.getName().c_str(),advertisedDevice.getRSSI()};
          myLinkedList ->add(item_temp);

          }
        
      }



    }
};



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Scanning...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
}

void loop() {
  // put your main code here, to run repeatedly:
  
   BLEDevice::init("");
  // put your main code here, to run repeatedly:
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);

  //Imprimir en JSON

  int listSize = myLinkedList->size();
  //Imprimit en JSON

    for (int h = 0; h < listSize; h++) {

      if(h==0){
        Serial.print("{\"devices\":");
      }

    item itemDevice = myLinkedList->get(h);
    
  Serial.print("{\"Found by\": \"");
    Serial.print(itemDevice.foundByNameDevice);
    Serial.print("\",");
    Serial.print("\"Device Found\": ");
    Serial.print(String(itemDevice.foundNameDevice));
    Serial.print(",");
    Serial.print("\"rssi\": \"");
    Serial.print(itemDevice.rssi);
    Serial.print("\"");
    

        if(h==listSize-1){
        Serial.println("}}");
      }
      else{
        Serial.print("\"},");
      }
  }
  
  myLinkedList->clear();
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(30000);
  BLEDevice::deinit(false);
}