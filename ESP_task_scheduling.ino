#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif
#define LED_BUILTIN 2

//Something print on screen
const char msg[] = "this is the demo on ESP32";

//Task handles
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

//pins
static const int led_pin = LED_BUILTIN;

//*******************************************************************************//
//TASKS


//Task: print to serial monitor with lower priority
void startTask1(void *parameter) {

  //count number of character in string
  int msg_len = strlen(msg);

  //print string to terminal
  while (1) {
    Serial.println();
    for (int i = 0; i < msg_len; i++) {
      Serial.print(msg[i]);
    }
    Serial.println();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

//Task: print to serial monitor with Higher priority
void startTask2(void *parameter) {

  //print string to terminal
  while (1) {
    Serial.print("*");
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}




void setup() {
  //configure serial to go slow
  Serial.begin(300);

  //wait a moment to start
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  Serial.println();
  Serial.println("---FreeRTOS Task DEMO---");


  //print sefl priority
  Serial.print("setup and loop task running on core");
  Serial.print(xPortGetCoreID());
  Serial.print(" with priority ");
  Serial.print(uxTaskPriorityGet(NULL));



  //Task to run forever

  xTaskCreatePinnedToCore(  //use xTaskCreate() in vanilla FreeRTOS
    startTask1,             //Function to be called
    "Task 1",               //Name of Task
    1024,                   //stack Size
    NULL,                   //parameter to pass
    1,                      //Task priority
    &task_1,                //Task handle
    app_cpu                 //run on one core
  );

  xTaskCreatePinnedToCore(  //use xTaskCreate() in vanilla FreeRTOS
    startTask2,             //Function to be called
    "Task 2",               //Name of Task
    1024,                   //stack Size
    NULL,                   //parameter to pass
    2,                      //Task priority
    &task_2,                //Task handle
    app_cpu                 //run on one core
  );
}

void loop() {
  // put your main code here, to run repeatedly:


  //suspend the higher priority task for some interval
  for (int i = 0; i < 3; i++) {
    vTaskSuspend(task_2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    vTaskResume(task_2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }

  //Deleted the lower priority task
  if (task_1 != NULL) {
    vTaskDelete(task_1);
    task_1 = NULL;
  }
}
