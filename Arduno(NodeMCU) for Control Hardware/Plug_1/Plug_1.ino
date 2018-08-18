#include <ESP8266WiFi.h>
#define SERVER_PORT 8000            //ค่า port ที่ต้องการเชื่อมต่อ
IPAddress server_ip = {192,168,2,101};       //ค่า ip ของ Server (อย่าลืมแก้เป็น IP ของตัวเอง)
 
const char* ssid = "Smarthome";         //ค่าของ SSID (อย่าลืมแก้เป็น ชื่อ SSID ของตัวเอง)
const char* password = "smarthome1234"; //ค่าของ SSID (อย่าลืมแก้เป็น password ของตัวเอง)
 
WiFiServer server(SERVER_PORT); //สร้าง object server และกำหนด port ที่ต้องการเชื่อมต่อกับ server
WiFiClient client;              //สร้าง object client
 
void setup() 
{
    pinMode(12, OUTPUT);
    pinMode(14, OUTPUT);
    digitalWrite(12, HIGH);
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
    digitalWrite(14,HIGH); 
//ESP.wdtDisable();  //ปิด watch dog Timer
}
 
void loop() 
{
  while (WiFi.status() != WL_CONNECTED) {
      digitalWrite(14,LOW); 
      delay(500);
      Serial.print(".");
    }
    while (client.status() == 0)  //รอการเชื่อมต่อ
    {
      digitalWrite(14,LOW); 
      client.connect(server_ip,SERVER_PORT);    
      delay(500);
      Serial.print(".");  
    }
    while(client.available())   //ตรวจเช็ตว่ามี Data ส่งมาจาก Server หรือไม่
    {   
            Serial.print("Message in coming !\n");
            client.println(" ");
            String line = client.readString();
            Serial.print(line);
            Serial.print("\n");
            if(line == "Plug/1/true"){
              Serial.print("Plug 1 is on!\n");
              digitalWrite(12, LOW);
              client.println("Thing_Plug/1/Confirm-true");
            }
            else if(line == "Plug/1/false" ){
              Serial.print("Plug 1 is off!\n");
              digitalWrite(12, HIGH);
              client.println("Thing_Plug/1/Confirm-false");
            } else if (line == "reset"){
              Serial.print("Plug 1 is Reset!\n");
              digitalWrite(12, HIGH);
              client.println("Thing_Plug/1/Confirm-reset");
            }
            
    }
               //client.println("Hello form client2");     //ส่งค่าที่ได้รับกลับไปยัง Server
               delay(200);
               digitalWrite(14,HIGH); 
}
