//Blink control using serial monitor.
#if CONFIG_FREERTOS_UNICORE
     static const BaseType_t app_core=0;
#else
     static const BaseType_t app_core=1;
#endif

static const int led=LED_BUILTIN;
int delayTime=1000;
void Blinky(void *parameter)
{
  while(1)
  {
    digitalWrite(led,HIGH);
    vTaskDelay(delayTime/portTICK_PERIOD_MS);
    digitalWrite(led,LOW);
    vTaskDelay(delayTime/portTICK_PERIOD_MS);
  }
}
void setup() {
  // put your setup code here, to run once:
  pinMode(led,OUTPUT);
  Serial.begin(9600);
  vTaskDelay(1000/portTICK_PERIOD_MS);
  Serial.println("Enter the delay :");
  xTaskCreatePinnedToCore(Blinky,"Blinky",1024,NULL,1,NULL,app_core);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()>0)
  {
    delayTime=Serial.parseInt();
  }
}
