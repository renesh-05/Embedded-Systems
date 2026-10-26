#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_core=0;
#else
static const BaseType_t app_core=1;
#endif

static BaseType_t input_size=0;
char *ptr=NULL;
static uint8_t n=0;
TaskHandle_t task1 =NULL;

void taskA(void *parameter)
{
  while(1)
  {
    if(Serial.available()>0)
    {
     input_size=Serial.available();
     Serial.print("Memory before allocation :");
     Serial.println(xPortGetFreeHeapSize());
     ptr=(char*)pvPortMalloc((input_size + 1)* sizeof(char));
     Serial.print("Memory after allocation :");
     Serial.println(xPortGetFreeHeapSize());
     if(ptr==NULL)
     {
      Serial.print("Not enough space in heap");
     }
     else
     {
      for(int i=0;i<input_size;i++)
      {
        ptr[i]=Serial.read();
      }
      ptr[input_size]='\0';
      n=1;
     }
     
    }
    vTaskDelay(10/portTICK_PERIOD_MS);
  }
}
void taskB(void *parameter)
{
  while(1)
  {
  if(n==1)
  {
    vTaskSuspend(task1);
    Serial.println(ptr);
    n=0;
    vPortFree(ptr);
    ptr=NULL;
    Serial.println("Enter a sentence :");
    vTaskResume(task1);
  }
  vTaskDelay(10/portTICK_PERIOD_MS);
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  vTaskDelay(1000/portTICK_PERIOD_MS);
  Serial.println("Enter a sentence :");
  xTaskCreatePinnedToCore(taskA,"Task A",1024,NULL,1,&task1,app_core);
  xTaskCreatePinnedToCore(taskB,"Task B",1024,NULL,1,NULL,app_core);
  vTaskDelete(NULL);
}

void loop() {
  // put your main code here, to run repeatedly:

}
