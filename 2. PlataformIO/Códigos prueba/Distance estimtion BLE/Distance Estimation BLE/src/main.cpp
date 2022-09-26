#include <Arduino.h>


#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

int scanTime = 5; //In seconds
BLEScan* pBLEScan;


	/* dBm to distance parameters; How to update distance_factor 1.place the
	 * phone at a known distance (2m, 3m, 5m, 10m) 2.average about 10 RSSI
	 * values for each of these distances, Set distance_factor so that the
	 * calculated distance approaches the actual distances, e.g. at 5m. */
	static constexpr float reference_power  = -50; //rssi reffrence 
	static constexpr float distance_factor = 3.5;

  static float get_distance(const int8_t rssi)
	{ return pow(10, (reference_power - rssi)/(10*distance_factor)); }

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      //Serial.printf("Advertised Device: %s \n", advertisedDevice.getAddress().toString().c_str());
      
      signed int rssi;
      if(advertisedDevice.getAddress().toString() == "30:c6:f7:2f:b1:de"){
        if (advertisedDevice.haveRSSI()){
          //Serial.printf("Rssi: %d \n", (int)advertisedDevice.getRSSI());          
          Serial.println("Se encontró nodo");      
          Serial.printf("Nodo: %s Rssi: %d \n", advertisedDevice.getName().c_str() , (int)advertisedDevice.getRSSI());
          rssi = (int)advertisedDevice.getRSSI();

          Serial.printf("La distancia es: %f \n", get_distance((int)advertisedDevice.getRSSI()));                   
        
          }
        //Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
      }


    }
};

void setup() {
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
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  //Serial.print("Devices found: ");
  //Serial.println(foundDevices.getCount());
  //Serial.println("Scan done!");
  //pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(2000);
}