#include <Arduino.h>

int count1 = 0;
int count2 = 0;

TaskHandle_t task1_handle = NULL; 

void task1(void * parameters){
  for(;;){
    Serial.print("Task 1 counter: ");
    Serial.println(count1++);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

      
  }
}

void task2(void * parameters){
  for(;;){
    Serial.print("Task 2 counter: ");
    Serial.println(count2++);
    vTaskDelay(1000 / portTICK_PERIOD_MS);    
  }
}


void setup() {
  // put your setup code here, to run once:
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