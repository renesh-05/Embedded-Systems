#if CONFIG_FREERTOS_UNICORE
 static const BaseType_t app_core=0;
#else
 static const BaseType_t app_core=1;
#endif

static const uint8_t led=LED_BUILTIN;
static TimerHandle_t mytime =NULL;

void timercallback1(TimerHandle_t xTimer)
{
  digitalWrite(led,1);
}

void task1(void *parameter)
{
  while(1)
  {
  if(Serial.available()>0)
  {
    char incoming_char = Serial.read();
    digitalWrite(led,0);
    xTimerStart(mytime,portMAX_DELAY);
  }
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  vTaskDelay(1000/portTICK_PERIOD_MS);
  pinMode(led,OUTPUT);

  mytime=xTimerCreate("OFF timer",5000/portTICK_PERIOD_MS,pdFALSE,(void *)0,timercallback1);

   if(mytime==NULL)
  {
    Serial.println("Timer creation failed.");
  }
  else
  {
    Serial.println("Timer starting.");
    xTimerStart(mytime,portMAX_DELAY);
  }
  xTaskCreatePinnedToCore(task1,"TASK 1",1024,NULL,1,NULL,app_core);
  vTaskDelete(NULL);
}

void loop() {
  // put your main code here, to run repeatedly:

}
