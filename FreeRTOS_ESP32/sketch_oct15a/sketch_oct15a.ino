#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_core=0;
#else
static const BaseType_t app_core=1;
#endif

const char msg[]="Hello how are you";

static TaskHandle_t task1 = NULL;
static TaskHandle_t task2 = NULL;

void tsk1(void *parameter)
{
  int msg_l = strlen(msg);
  Serial.println();
  while(1)
  {
  for(int i=0;i<msg_l;i++)
  {
    Serial.print(msg[i]);
  }
  Serial.println();
  vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}

void tsk2(void *parameter)
{
  while(1)
  {
  Serial.print("*");
  vTaskDelay(100/portTICK_PERIOD_MS);
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(300);

  vTaskDelay(1000/portTICK_PERIOD_MS);

  Serial.println();
  Serial.println("Hello! The code is working");
  Serial.print("The setup and loop running on core ");
  Serial.print(xPortGetCoreID());
  Serial.print(" and with priority level ");
  Serial.println(uxTaskPriorityGet(NULL));

  xTaskCreatePinnedToCore(tsk1,"Task 1",1024,NULL,1,&task1,app_core);
  xTaskCreatePinnedToCore(tsk2,"Task 2",1024,NULL,2,&task2,app_core);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0;i<3;i++)
  {
    vTaskSuspend(task2);
    vTaskDelay(2000/portTICK_PERIOD_MS);
    vTaskResume(task2);
    vTaskDelay(2000/portTICK_PERIOD_MS);

    if(task1!=NULL)
    {
      vTaskDelete(task1);
      task1=NULL;
    }
  }
}
