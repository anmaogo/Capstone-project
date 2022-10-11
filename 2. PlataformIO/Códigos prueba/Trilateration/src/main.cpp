#include <Arduino.h>
#include <Hello.h>
#include <Trilateration.h>




double x_1 = 2;
double y_1 = 5;
double r_1 = sqrt(5);

double x_2 = 0;
double y_2 = 0;
double r_2 = sqrt(16);

double x_3 = 4;
double y_3 = 3;
double r_3 = sqrt(5);



Hello hello;

Trilateration trilateration;

ulong current_time, prev_time;
uint16_t dt_ms = 500;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  prev_time = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  current_time = millis();
  if (current_time - prev_time > dt_ms)
  {
    prev_time = current_time;
    trilateration.Localizate(x_1, y_1, x_2, y_2, x_3, y_3, r_1, r_2, r_3);   
    
  }
  
  
}