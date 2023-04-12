#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif
#define LED_BUILTIN 2
//pins
static const int led_pin = LED_BUILTIN;


//our tast: blink an LED
void toggleLED(void* perameter) {
    while (1) {
        digitalWrite(led_pin, HIGH);
        vTaskDelay(1500 / portTICK_PERIOD_MS);
        digitalWrite(led_pin, LOW);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void setup() {
    // put your setup code here, to run once:
    pinMode(led_pin, OUTPUT);

    xTaskCreatePinnedToCore( //use xTaskCreate() in vanilla FreeRTOS
        toggleLED,     //Function to be called 
        "Toggle LED",  //Name of Task
        1024,          //stack Size
        NULL,          //parameter to pass
        1,             //Task priority
        NULL,          //Task handle
        app_cpu        //run on one core 
    );

}

void loop() {
    // put your main code here, to run repeatedly:
    //hello world
}