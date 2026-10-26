#include<stdlib.h>

#if CONFIG_FREERTOS_UNICORE
    static const BaseType_t app_core=0;
#else
    static const BaseType_t app_core =1;
#endif


static const uint8_t buf_len=255;
static QueueHandle_t queue1;
static QueueHandle_t queue2;

static const uint8_t queue1_len = 5;
static const uint8_t queue2_len = 30;

static const int led = LED_BUILTIN;

void task1(void *parameter)
{
  char c;
  char buf[buf_len];
  memset(buf,0,buf_len);
  uint8_t idx=0;
  while(1)
  {
   if(xQueueReceive(queue2,(void *)&buf,0)==pdTRUE)
    {
      Serial.println(buf);
      memset(buf,0,buf_len);
    } 
    if(Serial.available()>0)
    {
      c=Serial.read();
    
    if(idx<buf_len-1)
    {
      buf[idx]=c;
      idx++;
    }
    if(c=='\n')
    {
      buf[idx-1]='\0';
      if(buf[0]=='d'&& buf[1]=='e'&& buf[2]=='l' && buf[3]=='a' && buf[4]=='y')
      {
        char a[5];
        memset(a,0,5);
        int i=6;
        int b=0;
        while(buf[i]!='\0')
        {
          a[b]=buf[i];
          i++;
          b++;
        }
        a[b]='\0';
        int dela = atoi(a);
        if(xQueueSend(queue1,(void *)&dela,10)==pdTRUE)
        {
          Serial.println("Message sent!");
        }
      }
      Serial.println(buf);
      memset(buf,0,buf_len);
      idx=0;
    }
    }
    vTaskDelay(100/portTICK_PERIOD_MS);
  }
}

void task2(void *parameter)
{
  int del=500;
  int c=0;
  char s[30]="LED BLINKED 100 TIMES";
  while(1)
  {
  if(xQueueReceive(queue1,(void *)&del,0)==pdTRUE)
  {
    ;
  }
  digitalWrite(led,1);
  c++;
  vTaskDelay(del/portTICK_PERIOD_MS);
  digitalWrite(led,0);
  vTaskDelay(del/portTICK_PERIOD_MS);
  if(c==100)
  {
    xQueueSend(queue2,(void *)&s,10);
    c=0;
  }
}
}
void setup() {
  // put your setup code here, to run once:
  pinMode(led,OUTPUT);
  Serial.begin(9600);
  queue1=xQueueCreate(queue1_len,sizeof(int));
  queue2=xQueueCreate(queue2_len,sizeof(char[30]));
  xTaskCreatePinnedToCore(task1,"Task 1",1024,NULL,1,NULL,app_core);
  xTaskCreatePinnedToCore(task2,"Task 2",1024,NULL,1,NULL,app_core);
  vTaskDelete(NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
