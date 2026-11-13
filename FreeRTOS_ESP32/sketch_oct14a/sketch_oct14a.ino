//First task to blink LED using FreeRTOS
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_core = 0;
#else
static const BaseType_t app_core=1;
#endif

static const int led=LED_BUILTIN;
void blinky(void *param)
{
  while(1)
  {
    digitalWrite(led,HIGH);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    digitalWrite(led,LOW);
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}
void blinky1(void *param)
{
  while(1)
  {
    digitalWrite(led,HIGH);
    vTaskDelay(1800/portTICK_PERIOD_MS);
    digitalWrite(led,LOW);
    vTaskDelay(1800/portTICK_PERIOD_MS);
  }
}
void setup() {
  // put your setup code here, to run once:
  pinMode(led,OUTPUT);
  xTaskCreatePinnedToCore(blinky,
                          "Blink LED",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_core);
  xTaskCreatePinnedToCore(blinky1,
                          "Blink 1 LED",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_core);
}

void loop() {
  // put your main code here, to run repeatedly:

}
