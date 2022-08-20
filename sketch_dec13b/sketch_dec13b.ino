#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#define DEBUG 1
#define LED 2 
#define D0 -1   //音符对应的相应频率
#define D1 523  //NOTE_D表示D调
#define D2 587
#define D3 659
#define D4 698
#define D5 784
#define D6 880
#define D7 988

#define DL1 261 //NOTE_DL表示D调LOW
#define DL2 294
#define DL3 329
#define DL4 349
#define DL5 392
#define DL6 440
#define DL7 493

#define DH1 1046//NOTE_DH表示D调HIGH
#define DH2 1175
#define DH3 1318
#define DH4 1397
#define DH5 1568
#define DH6 1760
#define DH7 1976
#define Speed 600

#include <SoftwareSerial.h>
SoftwareSerial ESP_Serial(2,3);

int tuneqinghuaci[] = 
{
  D0,D5,D5,D3,D2,D3,DL6,D2,D3,D5,D3,D2,D0,D5,D5,D3,D2,D3,DL5,D2,D3,D5,D2,D1,
  D0,D1,D2,D3,D5,D6,D5,D3,D5,D3,D3,D2,D2,D0,D1,D2,D1,D2,D1,D2,D3,D5,D3,
  D0,D5,D5,D3,D2,D3,DL6,D2,D3,D5,D3,D2,D0,D5,D5,D3,D2,D3,DL5,D2,D3,D5,D2,D1,
  D0,D1,D2,D3,D5,D6,D5,D3,D5,D3,D3,D2,D2,DL5,D3,D2,D2,D1
};

int durationqinghuaci[] = 
{
  1,1,1,1,1,1,2,1,1,1,1,4,1,1,1,1,1,1,2,1,1,1,1,4,
  1,1,1,1,1,1,1,1,1,1,1,1,4,1,1,1,1,2,1,2,2,2,3,
  1,1,1,1,1,1,2,1,1,1,1,4,1,1,1,1,1,1,2,1,1,1,1,4,
  1,1,1,1,1,1,1,1,1,1,1,1,3,1,2,1,2,6
};

int lengthqinghuaci;

int tunedaoxiang[]
{
  D3,D4,D5,D5,D5,D5,D5,D5,D5,D5,D5,D2,D3,D2,D1,
  D1,D2,D3,D3,D3,D3,D3,D3,D2,D3,D1,DL6,
  DL6,D1,D1,D1,D1,D1,D2,D2,D2,D1,D2,D3,D3,D0,
  D3,D4,D5,D5,D5,D5,D5,D5,D5,D5,D5,D2,D3,D2,D1,
  D1,D2,D3,D3,D3,D3,D3,D3,D2,D3,D1,DL6,
  DL6,D1,D1,D1,D1,D2,D2,D2,D2,D1,D1,D1
};

float durationdaoxiang[]
{
  1,0.5,0.5,1,0.5,1,1,1,0.5,0.5,0.5,0.5,0.5,0.5,0.5,
  1,0.5,1,1,1,0.5,0.5,0.5,0.5,0.5,0.5,0.5,
  1,0.5,1,0.5,0.5,0.5,0.5,0.5,0.5,1,0.5,0.5,4,2,
  1,0.5,0.5,1,0.5,1,1,1,0.5,0.5,0.5,0.5,0.5,0.5,0.5,
  1,0.5,1,1,1,0.5,0.5,0.5,0.5,0.5,0.5,0.5,
  1,0.5,1,0.5,0.5,1,0.5,0.5,0.5,0.5,1,8
};

int lengthdaoxiang;

int tuneseeyouagain[]
{
  D1,D3,D5,D6,D5,D0,D1,D2,D2,D1,D2,D3,D0,
  D3,D5,D6,D7,D6,D5,D3,D2,D2,D1,D2,D2,D2,D1,D0,
  D0,D1,D3,D5,D6,D5,D5,D0,D1,D2,D2,D1,D3,D0,
  D2,D3,D5,D6,DH1,DH2,DH3,DH2,DH1,D5,D6,DH1,DH2,DH2,DH2,DH1,D0,
  D5,D6,DH1,DH2,DH2,DH2,DH1,D0,D0
};

float durationseeyouagain[]
{
  1.2,1.2,1.2,3.6,3.6,2.4,0.6,1.2,1.2,1.2,1.2,2.4,1.2,
  0.6,0.6,1.2,1.2,1.2,1.2,1.2,1.2,1.2,1.2,1.2,1.2,1.2,1.2,2.4,
  0.6,0.6,0.6,0.6,3.6,3.6,2.4,0.6,1.2,1.2,1.2,1.2,2.4,
  0.6,0.6,1.2,1.2,1.2,1.2,1.2,1.2,1.2,0.6,0.6,1.2,1.2,1.2,1.2,1.2,2.4,
  0.6,0.6,1.2,1.2,1.2,1.2,1.2,2.4,1.2,1.2
};

int lengthseeyouagain;
int tonePin=5;//蜂鸣器的pin

const char ssid[]     = "TP-LINK_403";                   // 使用时请修改为当前你的 wifi 名称
const char password[] = "19731016";                      // 使用时请修改为当前你的 wifi 密码
const char OneNetServer[] = "api.heclouds.com";          // OneNet服务器域名
const char APIKEY[] = "HwbUcYVNVNMd1EZzxKTpmgm2=dM=";    // 使用时请修改为你的API KEY
int32_t DeviceId = 643323496;                            // 使用时请修改为你的设备ID
char DataStreams[] = "switch";                           // 数据流
const size_t MAX_CONTENT_SIZE = 1024;                    // 最大内容长度
const unsigned long HTTP_TIMEOUT = 21000;                // 超时时间

int stream1;                     //用于存放返回"switch"的数值

WiFiClient client;
const int tcpPort = 80;

struct UserData 
{
    int errno_val;               // 错误返回值
    char error[32];              // 错误返回信息
    int recived_val;             // 接收数据值
    char udate_at[32];           // 最后更新时间及日期
};

//跳过 HTTP 头，使我们在响应正文的开头
bool skipResponseHeaders() 
{
    char endOfHeaders[] = "\r\n\r\n";
    client.setTimeout(HTTP_TIMEOUT);
    bool ok = client.find(endOfHeaders);
    if (!ok) 
    {    }
    return ok;
}

//从HTTP服务器响应中读取正文
void readReponseContent(char* content, size_t maxSize) 
{
    size_t length = client.readBytes(content, maxSize);
    delay(20);
    content[length] = 0;
}

// 解析数据存储到传入的结构体中
bool parseUserData_test(char* content, struct UserData* userData) 
{
    // 根据我们需要解析的数据来计算JSON缓冲区最佳大小
    // 如果你使用StaticJsonBuffer时才需要
    const size_t BUFFER_SIZE = 1024;
    // 在堆栈上分配一个临时内存池
    StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
    // 如果堆栈的内存池太大，使用 DynamicJsonBuffer jsonBuffer 代替
    JsonObject& root = jsonBuffer.parseObject(content);
    if (!root.success()) 
    {
     // Serial.println("JSON parsing failed!");
      return false;
    }
    // 复制需要的信息到userData结构体中
    userData->errno_val = root["errno"];
    strcpy(userData->error, root["error"]);
    // 成功返回0 
    if ( userData->errno_val == 0 ) 
    {
      userData->recived_val = root["data"]["datastreams"][0]["datapoints"][0]["value"];
      strcpy(userData->udate_at, root["data"]["datastreams"][0]["datapoints"][0]["at"]);
    }
    return true;
}

// 读取数据
int readData(int dId, char dataStream[])
{
    // 创建发送请求的URL
    String url = "/devices/";
    url += String(dId);
    url += "/datapoints?datastream_id=";
    url += dataStream;

    // 创建发送指令
    String send_data = String("GET ") + url + " HTTP/1.1\r\n" +
                     "api-key:" + APIKEY + "\r\n" +
                     "Host:" + OneNetServer + "\r\n" +
                     "Connection: close\r\n\r\n";
    // 发送指令
    client.print(send_data);
    // 调试模式串口打印发送的指令
    if (DEBUG)
    {    }
    unsigned long timeout = millis();
    while (client.available() == 0) 
    {
      if (millis() - timeout > 2000) 
      {
        client.stop();
        break;
      }      
    }

    if (skipResponseHeaders())  
    { 
      char response[MAX_CONTENT_SIZE];
      // 从服务器读取信息后解析
      readReponseContent(response, sizeof(response));
      UserData userData_LEDstatus;
      if (parseUserData_test(response, &userData_LEDstatus)) 
      {
        return userData_LEDstatus.recived_val;
      }
     }
 }

void setup() 
{
  WiFi.mode(WIFI_AP_STA);               //设置工作模式
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  delay(10);


  // 连接到wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
  }
  ESP_Serial.begin(115200);  //定义软串口波特率
  pinMode(tonePin,OUTPUT);//设置蜂鸣器的pin为输出模式
  lengthqinghuaci = sizeof(tuneqinghuaci)/sizeof(tuneqinghuaci[0]);
  lengthdaoxiang = sizeof(tunedaoxiang)/sizeof(tunedaoxiang[0]);
  lengthseeyouagain = sizeof(tuneseeyouagain)/sizeof(tuneseeyouagain[0]);
}

void loop() 
{
    //建立连接并判断
    if (!client.connect(OneNetServer, tcpPort)) 
    {
      return;
    }
    //从云端获取值并存于stream1
    stream1=readData(DeviceId, DataStreams);
    Serial.println(stream1);
    stream1 = ESP_Serial.parseInt();
    Serial.println(stream1);
    if(stream1==1)
    {
      for(int x=0;x<lengthqinghuaci && stream1 != 2 && stream1 != 3 &&stream1!=4; x++)//循环音符的次数
      {      
  
          if(x%8==0)
          {stream1 = ESP_Serial.parseInt();}
          Serial.println(stream1);
          tone(tonePin,tuneqinghuaci[x]);//此函数依次播放tune序列里的数组，即每个音符
          delay(Speed*durationqinghuaci[x]);//每个音符持续的时间，即节拍duration，调整时间越大，曲子速度越慢；越小曲子速度越快
          noTone(tonePin);//停止当前音符，进入下一音符
      }
    }
    if(stream1==2)
    {
      for(int x=0;x<lengthdaoxiang && stream1 != 1 && stream1 != 3&&stream1!=4; x++)//循环音符的次数
      {   
          if(x%16==0)
          {stream1 = ESP_Serial.parseInt();}
          Serial.println(stream1);
          tone(tonePin,tunedaoxiang[x]);//此函数依次播放tune序列里的数组，即每个音符
          delay(Speed*durationdaoxiang[x]);//每个音符持续的时间，即节拍duration，调整时间越大，曲子速度越慢；越小曲子速度越快
          noTone(tonePin);//停止当前音符，进入下一音符
      }
    }
    if(stream1==3)
    {
      for(int x=0;x<lengthseeyouagain && stream1 != 1 && stream1 != 2&&stream1!=4; x++)//循环音符的次数
      {   
        
          Serial.println(stream1);
          tone(tonePin,tuneseeyouagain[x]);//此函数依次播放tune序列里的数组，即每个音符
          if(x%4==0)
          {stream1 = ESP_Serial.parseInt();}
          delay(Speed*durationseeyouagain[x]);//每个音符持续的时间，即节拍duration，调整时间越大，曲子速度越慢；越小曲子速度越快
          noTone(tonePin);//停止当前音符，进入下一音符
      }
    }
}
