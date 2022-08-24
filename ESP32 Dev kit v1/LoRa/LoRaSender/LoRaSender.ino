#include <SPI.h>
#include <LoRa.h>
#define led 2                                // LED utilizado para notificar la emisión de un mensaje.
int counter = 0;

void setup() {
  Serial.begin(115200);
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

  delay(5000);
}
