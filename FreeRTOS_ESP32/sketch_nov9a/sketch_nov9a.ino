#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_core=0;
#else
static const baseType_t app_core=1;
#endif

uint8_t led=LED_BUILTIN;
static SemaphoreHandle_t mutex;

void task1(void *parameter)
{
  xSemaphoreTake(mutex,portMAX_DELAY);
  int local_var =0;
  vTaskDelay(1000/portTICK_PERIOD_MS);
  local_var = *(int *)parameter;
  xSemaphoreGive(mutex);
  
  Serial.print("Recieved num :");
  Serial.println(local_var);
  pinMode(led,OUTPUT);
  
  while(1)
  {
  digitalWrite(led,1);
  vTaskDelay(local_var/portTICK_PERIOD_MS);
  digitalWrite(led,0);
  vTaskDelay(local_var/portTICK_PERIOD_MS);
  }
  
}
void setup() {
  // put your setup code here, to run once:
  long int delay_time;

  
  Serial.begin(9600);

  vTaskDelay(1000/portTICK_PERIOD_MS);

  while(Serial.available()<=0);
  delay_time=Serial.parseInt();
  Serial.print("Sending :");
  Serial.println(delay_time);
  mutex=xSemaphoreCreateMutex();
  
  xTaskCreatePinnedToCore(task1,"Task 1",4096,(void *)&delay_time,1,NULL,app_core);
   
  vTaskDelay(portTICK_PERIOD_MS);
  xSemaphoreTake(mutex,portMAX_DELAY);
  Serial.println("Done");
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
