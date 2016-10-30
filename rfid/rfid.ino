#include <SPI.h>
#include <RFID.h>
#include <Servo.h> 

Servo myservo;  //创建一个舵机控制对象

                       // 使用Servo类最多可以控制8个舵机

int pos = 0;


//D10 - 读卡器CS引脚、D5 - 读卡器RST引脚
RFID rfid(10,5);   
unsigned char status;
unsigned char str[MAX_LEN];  //MAX_LEN为16，数组最大长度

void setup()
{ pinMode(13,OUTPUT);
  Serial.begin(9600);
  SPI.begin();
  rfid.init(); //初始化
  digitalWrite(13,1);
  delay(1000);
  digitalWrite(13,0);
  myservo.attach(9);  
}

void loop()
{
  //Search card, return card types
  if (rfid.findCard(PICC_REQIDL, str) == MI_OK) {
    Serial.println("Find the card!");
    digitalWrite(13,HIGH);
    // Show card type
    ShowCardType(str);
    //防冲突检测,读取卡序列号
    if (rfid.anticoll(str) == MI_OK) {
      Serial.print("The card's number is  : ");
      //显示卡序列号
      for(int i = 0; i < 4; i++)
      {
        Serial.print(0x0F & (str[1] >> 4));
        Serial.print(0x0F & str[1]);
        Serial.print(rfid.anticoll(str));
        
      }
    Serial.print(str[1]);
     
     
      Serial.println("");
    }
    //选卡（锁定卡片，防止多数读取，去掉本行将连续读卡）
    rfid.selectTag(str);
      if(str[1]==153||str[1]==233||str[1]==53||str[1]==24)
      {digitalWrite(13,1);
      delay(300);
      digitalWrite(13,0);
      delay(300);
      digitalWrite(13,1);
      delay(300);
      digitalWrite(13,0);
       Serial.println("abc");
       for(pos = 0; pos < 180; pos += 1)  // 从0度到180度运动 

  {                                                     // 每次步进一度

   myservo.write(pos);        // 指定舵机转向的角度

  delay(15);                       // 等待15ms让舵机到达指定位置

  } 

 for(pos = 180; pos>=1; pos-=1)   //从180度到0度运动  

 {                                

  myservo.write(pos);         // 指定舵机转向的角度 

  delay(15);         

      }

    
  }
  
  rfid.halt();  //命令卡片进入休眠状态
}}

void ShowCardType(unsigned char * type)
{
  Serial.print("Card type: ");
  if(type[0]==0x04&&type[1]==0x00) 
    Serial.println("MFOne-S50");
  else if(type[0]==0x02&&type[1]==0x00)
    Serial.println("MFOne-S70");
  else if(type[0]==0x44&&type[1]==0x00)
    Serial.println("MF-UltraLight");
  else if(type[0]==0x08&&type[1]==0x00)
    Serial.println("MF-Pro");
  else if(type[0]==0x44&&type[1]==0x03)
    Serial.println("MF Desire");
  else
    Serial.println("Unknown");
}
