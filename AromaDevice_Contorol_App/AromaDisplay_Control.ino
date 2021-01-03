#include <WiFi.h>

WiFiServer wifiserver(80);

const char* ssid = "Your ssid";
const char* password = "Your password";

void setup()
{
  //Channel,PWM Frequneccy,Bit Number
  ledcSetup(0,110000,9);  //Left Atomizer 
  ledcSetup(1,110000,9);  //Right Atomizer 
  
  pinMode(25,OUTPUT); //左のポンプのスイッチング
  pinMode(26,OUTPUT); //右のポンプのスイッチング

  pinMode(4,OUTPUT); //左のLED点灯
  pinMode(2,OUTPUT); //右のLED点灯
  
  Serial.begin(115200); //シリアル通信速度設定
  while (!Serial);

  WiFi.begin(ssid, password);
  Serial.print("WiFi connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }

  Serial.println(" connected");

  wifiserver.begin();

  Serial.print("Connected to WiFi. IP:");
  Serial.print(WiFi.localIP());
}

void loop()
{
  WiFiClient client = wifiserver.available();
  if (client) {
    while (client.connected()) {
      while (client.available() > 0) {
        char c = client.read();
        Serial.write(c);
      }
      delay(10);
    }
    client.stop();
    Serial.println("Client disconnected");
  }
  
  int inputkey;
  inputkey = Serial.read();
  if (inputkey != -1){ 
    switch(inputkey){ 
      case 'a':
        Serial.println("左のアトマイザーとポンプを5秒間駆動");
        digitalWrite(25,HIGH);
        digitalWrite(4,HIGH);
  
        //Pin number,Channnel
        ledcAttachPin(16, 0);
        
        //Channnel,Duty Duty比：50%
        ledcWrite(0,256); 
  
        delay(15000);
        ledcDetachPin(16);
        digitalWrite(25,LOW);
        digitalWrite(4,LOW);
        Serial.println("終了");
        break;
      
      case 'k':
        Serial.println("右のアトマイザーとポンプを5秒間駆動");
        digitalWrite(26,HIGH);
        digitalWrite(2,HIGH);
  
        //Pin number,Channnel
        ledcAttachPin(17, 1); 
        
        //Channnel,Duty
        ledcWrite(1,256); 
  
        delay(5000);
        ledcDetachPin(17);
        digitalWrite(26,LOW);
        digitalWrite(2,LOW);
        Serial.println("終了");
        break;

      //実験用,デモ用
      case 'g':
        Serial.println("左 60秒駆動");
        digitalWrite(25,HIGH);
        digitalWrite(4,HIGH);
  
        //Pin number,Channnel
        ledcAttachPin(16, 0);
        
        //Channnel,Duty
        ledcWrite(0,256); 
  
        delay(60000UL);
        ledcDetachPin(16);
        digitalWrite(25,LOW);
        digitalWrite(4,LOW);
        Serial.println("終了");
        break;

      case 'j':
        Serial.println("ポンプ0.5秒,アトマイザー4.5秒");
        digitalWrite(25,HIGH);
        digitalWrite(4,HIGH);
  
        //Pin number,Channnel
        ledcAttachPin(16, 0);
        
        //Channnel,Duty
        ledcWrite(0,256); 

        delay(20);
        digitalWrite(25,LOW);
        Serial.println("ポンプ駆動 終了");
        
        delay(1480);
        ledcDetachPin(16);
        digitalWrite(4,LOW);
        Serial.println("アトマイザー駆動 終了");
        break;

      case 'v':
        Serial.println("ポンプ0.1秒,アトマイザー2.9秒 を4回繰り返す");
        for (int i = 0; i < 4; i++) {
          digitalWrite(25,HIGH);
          digitalWrite(4,HIGH);
  
          //Pin number,Channnel
          ledcAttachPin(16, 0);
        
          //Channnel,Duty
          ledcWrite(0,256); 

          delay(20);
          digitalWrite(25,LOW);
        
          delay(1480);
          ledcDetachPin(16);
          digitalWrite(4,LOW);
        }
        Serial.println("終了");
        break;
    }
  }
}
