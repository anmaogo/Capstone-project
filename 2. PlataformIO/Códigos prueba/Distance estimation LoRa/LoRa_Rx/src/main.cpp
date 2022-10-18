#include <Arduino.h>

#include <SPI.h>
#include <LoRa.h>

#define led 2                                  // LED utilizado para notificar la recepción de un mensaje.



static constexpr float reference_power  = -23.703; //rssi reffrence 
static constexpr float distance_factor = 3.286;

static float get_distance(const int8_t rssi)
{ return pow(10, (reference_power - rssi)/(10*distance_factor)); }

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Receiver");

   LoRa.setPins(17,16,22);                       // NSS, RESET y DIO0.

  while (!Serial);
  Serial.println("LoRa Receiver");
  if (!LoRa.begin(433E6)) { // or 915E6
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  pinMode(led,OUTPUT);
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");


    digitalWrite(led,HIGH);                     // Parpadear LED al recibir el mensaje.
    delay(100);
    digitalWrite(led,LOW);

    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());

    Serial.printf("La distancia es: %f \n", get_distance(LoRa.packetRssi()));   
  }
}