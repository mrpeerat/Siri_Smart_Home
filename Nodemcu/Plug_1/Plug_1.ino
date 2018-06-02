#include <ESP8266WiFi.h>
#define SERVER_PORT 8000            //ค่า port ที่ต้องการเชื่อมต่อ
IPAddress server_ip = {192,168,137,1};       //ค่า ip ของ Server (อย่าลืมแก้เป็น IP ของตัวเอง)
 
const char* ssid = "PingWiFi";         //ค่าของ SSID (อย่าลืมแก้เป็น ชื่อ SSID ของตัวเอง)
const char* password = "ping123456789"; //ค่าของ SSID (อย่าลืมแก้เป็น password ของตัวเอง)
 
WiFiServer server(SERVER_PORT); //สร้าง object server และกำหนด port ที่ต้องการเชื่อมต่อกับ server
WiFiClient client;              //สร้าง object client
 
void setup() 
{
    pinMode(13, OUTPUT);
    pinMode(15, OUTPUT);
    digitalWrite(13, HIGH);
    Serial.begin(115200);       //เปิดใช้ Serial
    WiFi.begin(ssid, password); //เชื่อมต่อกับ AP
   
     while (WiFi.status() != WL_CONNECTED)  //รอการเชื่อมต่อ
    {
            delay(500);
            Serial.print(".");
    }
   
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: "); 
    Serial.println(WiFi.localIP()); //แสดงหมายเลข IP
 
    Serial.println("Connect TCP Server");
    while (!client.connect(server_ip,SERVER_PORT)) //เชื่อมต่อกับ Server
    {
            Serial.print(".");
            delay(100);
    } 
    Serial.println("Success");
    digitalWrite(15,HIGH); 
//ESP.wdtDisable();  //ปิด watch dog Timer
}
 
void loop() 
{
    while(client.available())   //ตรวจเช็ตว่ามี Data ส่งมาจาก Server หรือไม่
    {   
            Serial.print("Message in coming !\n");
            String line = client.readString();
            Serial.print(line);
            Serial.print("\n");
            if(line == "Plug/1/true"){
              Serial.print("Plug 1 is on!\n");
              digitalWrite(13, LOW);
              client.println("Thing_Plug/1/Confirm-true");
            }
            else if(line == "Plug/1/false" ){
              Serial.print("Plug 1 is off!\n");
              digitalWrite(13, HIGH);
              client.println("Thing_Plug/1/Confirm-false");
            } else if (line == "reset"){
              Serial.print("Plug 1 is Reset!\n");
              digitalWrite(13, HIGH);
              client.println("Thing_Plug/1/Confirm-reset");
            }
            
    }
               //client.println("Hello form client1");     //ส่งค่าที่ได้รับกลับไปยัง Server
               delay(100);
}
