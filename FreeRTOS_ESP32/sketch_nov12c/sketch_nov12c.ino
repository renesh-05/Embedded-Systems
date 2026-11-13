#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_core=0;
#else
static const BaseType_t app_core=1;
#endif

static const uint16_t prescaler = 80;
static const uint64_t time_max = 100000;
static volatile float average =0;
static const uint8_t adc_pin = A0;

static volatile BaseType_t bufA[10];
static volatile BaseType_t bufB[10];
static uint8_t buf_select =0;
static uint8_t buf_idx = 0;

static hw_timer_t *timer = NULL;
static SemaphoreHandle_t sem_bin;
static SemaphoreHandle_t mutex;

void IRAM_ATTR ontime()
{
  BaseType_t task_woken = pdFALSE;
  if(buf_select==0)
  {
    bufA[buf_idx]=analogRead(adc_pin);
  }
  else
  {
    bufB[buf_idx]=analogRead(adc_pin);
  }
  
  if(buf_idx==9)
  {
    buf_select = 1 - buf_select;
    xSemaphoreGiveFromISR(sem_bin,&task_woken);
  }
  buf_idx=(buf_idx+1)%10;
  if(task_woken)
  {
    portYIELD_FROM_ISR();
  }
}

void TaskA(void *parameter)
{
  while(1)
  {
    xSemaphoreTake(sem_bin,portMAX_DELAY);
    float avg =0;
    if(buf_select==1)
    {
      for(int i=0;i<10;i++)
      {
        avg += bufA[i];
      }
    }
    else
    {
      for(int i= 0;i<10;i++)
      {
        avg += bufB[i];
      }
    }
    xSemaphoreTake(mutex,portMAX_DELAY);
    average =avg/10.0;
    xSemaphoreGive(mutex);
    //Serial.println(average);

    /*Serial.println("Start AAAAA");
    for(int i=0;i<10;i++)
    {
      Serial.println(bufA[i]);
    }
    memset(bufA,0,sizeof(bufA));
    Serial.println("End AAAA");

    Serial.println("Start BBBB");
    for(int i=0;i<10;i++)
    {
      Serial.println(bufB[i]);
    }
    memset(bufB,0,sizeof(bufB));
    Serial.println("End BBBB");*/
  }
}

void TaskB(void *parameter)
{
  char c=0;
  char buf[256];
  char cmd[]="avg";
  uint8_t idx=0;
  while(1)
  {
    if(Serial.available()>0)
    {
      c=Serial.read();
      buf[idx]=c;
      idx++;
      if(c=='\n')
      {
      buf[idx-1]='\0';
      idx=0;
      if(memcmp(buf,cmd,sizeof(cmd))==0)
      {
        xSemaphoreTake(mutex,portMAX_DELAY);
        Serial.print("Average : ");
        Serial.println(average);
        xSemaphoreGive(mutex);
      }
      else
      {
        Serial.println(buf);
      }
      memset(buf,0,sizeof(buf));
      }
      
    }
    
    
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
  
}
void setup()
{
  Serial.begin(9600);
  pinMode(A0,INPUT);
  vTaskDelay(1000/portTICK_PERIOD_MS);

  sem_bin= xSemaphoreCreateBinary();
  mutex = xSemaphoreCreateMutex();
  if(sem_bin==NULL)
  {
    Serial.println("Could not able to create Semaphore");
  }
  
  xTaskCreatePinnedToCore(TaskA,"Task A",4096,NULL,1,NULL,app_core);
  xTaskCreatePinnedToCore(TaskB,"Task B",4096,NULL,1,NULL,app_core);

  timer = timerBegin(1000000);
  timerAttachInterrupt(timer,&ontime);
  timerAlarm(timer,time_max,true,0);

  
}
void loop()
{

}

