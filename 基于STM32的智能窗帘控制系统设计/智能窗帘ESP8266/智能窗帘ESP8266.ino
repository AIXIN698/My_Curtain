
/*
 * 阿正整理设计
 * 此代码为 7天Java0基础速成安卓开发配套硬件代码
 * esp环境需要自己搭建（不同版本函数参数略有不同，自己优化）
 * 为达到硬件也是0基础（设计如下内容一个开发板一根数据线就可以完成）
 * 采用NodeMCU（或者任意ESP8266开发板）
 * 温度上报使用ADC模拟随机数
 * 数据接收不采用json解析而是直接字符串处理
 * 
 * 博客：http://wenzheng.club/
 * B站：https://space.bilibili.com/265908761
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>
WiFiClient espClient;
PubSubClient client(espClient);
const char* wifissid = "小米手机"; //改成自己家wifi
const char* password = "66666666"; //改成自己家wifi
const char* mqtt_server = "test.ranye-iot.net";
const char* mqtt_id = "1353023461_ESP";   //改成自己的QQ号+_ESP
const char* Mqtt_sub_topic = "1353023461_ESP";   //改成自己的QQ号+_ESP
const char* Mqtt_pub_topic = "1353023461";  //  上报消息给  手机APP的TOPIC  //改成自己的QQ号
long lastMsg = 0; //定时用的
int wifi_flag = 0; // wifi连接状态

String curtain_open = "0x20,0x22,0x00,0x01";   // 打开窗帘
String curtain_close = "0x20,0x22,0x00,0x02";   // 关闭窗帘
String comdata = "";
char red_Light;

void setup() {
  pinMode(2, OUTPUT);     
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {
//  Serial.println();
//  Serial.print("Connecting to ");
//  Serial.println(wifissid);
  WiFi.begin(wifissid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    wifi_flag = 0;
    digitalWrite(2,1);
  }
  wifi_flag = 1;
  digitalWrite(2,0);
//  Serial.println("");
//  Serial.println("WiFi connected");
//  Serial.println("IP address: ");
//  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  String msg="";
  String LED_set = "";
  String curtain_set = "";
  //Serial.print("Message arrived [");
  //Serial.print(topic);
  //Serial.print("] ");
  for (int i = 0; i < length; i++) {
    msg+= (char)payload[i];
  }
  //Serial.println(msg);

  if(msg.substring(6,9) == "led"){   //判断是否是要设置LED灯
    //取出LED_set数据 并执行 
    LED_set = msg.substring(msg.indexOf("led\":")+5,msg.indexOf("}")); 
    digitalWrite(2,!LED_set.toInt()); 
  }

  // String oc = msg.substring(6,13);   // 测试 提取发送过来的字符串的索引6~13中的字符
  if(msg.substring(6,13) == "curtain"){
    curtain_set = msg.substring(msg.indexOf("curtain\":")+9,msg.indexOf("}")); 
    if(curtain_set == " \"fA"){
      Serial.print(curtain_set);
    }else if(curtain_set == " \"fB"){
      Serial.print(curtain_set);
    }
  }

}


void reconnect() {
  while (!client.connected()) {
    //Serial.print("Attempting MQTT connection...");
    if (client.connect(mqtt_id)) {
      //Serial.println("connected");
      //连接成功以后就开始订阅
      client.subscribe(Mqtt_sub_topic,1);
    } else {
      //Serial.print("failed, rc=");
      //Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  
    while (Serial.available() > 0){
        comdata += char(Serial.read());  //每次读一个char字符，并相加
        delay(2);
    }
    if (comdata.length() > 0){
        //Serial.println(comdata); //打印接收到的字符
        String Light_value = comdata.substring(comdata.indexOf("Light\":")+17,comdata.indexOf("}")); 
        String Light_l = "{\"Light_flag\":"+String(Light_value)+"}";
        client.publish(Mqtt_pub_topic,Light_l.c_str());
        comdata = "";
    }


  while(Serial.available() > 0)
  {
     comdata += char(Serial.read());
  }
  if(sizeof(comdata) >= 15)
  {
    comdata =  "";   // 清空数据等待下一次接收
  }else if(sizeof(comdata) <= 12)
  {
     Serial.println("shuju = " + comdata);
  }
 
  comdata= "";   // 清空数据等待下一次接收
  delay(500);

  
  // 这里设置上传数据时间
  if (now - lastMsg > 500) {
    
  /* ---------------------------------------------------------------- */
    // 向APP上传wifi连接状态
    if(wifi_flag == 1){
      String wifi_h = "{\"wifi_flag\":"+String(wifi_flag)+"}";
      client.publish(Mqtt_pub_topic,wifi_h.c_str());
    }else{
      String wifi_l = "{\"wifi_flag\":"+String(wifi_flag)+"}";
      client.publish(Mqtt_pub_topic,wifi_l.c_str());
    }
    /* ---------------------------------------------------------------- */
    
     
  /* ---------------------------------------------------------------- */
    // 向APP上传LED状态数据
    // 读取输入引脚的值
    int buttonState = digitalRead(2);
    // 打印结果到串口
    //Serial.println(buttonState);
    String led_f = "{\"led_flag\":"+String(buttonState)+"}";
    client.publish(Mqtt_pub_topic,led_f.c_str());
    /* ---------------------------------------------------------------- */

    
  /* ---------------------------------------------------------------- */
    // 向APP上传ADC数据（模拟温度）
    lastMsg = now;
    String json = "{\"temperature\":"+String(analogRead(A0))+"}";
    client.publish(Mqtt_pub_topic,json.c_str());
    /* ---------------------------------------------------------------- */
    
  }
}
