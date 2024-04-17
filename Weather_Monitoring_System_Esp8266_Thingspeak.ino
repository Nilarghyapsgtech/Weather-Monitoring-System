#include <SoftwareSerial.h>
#include "DHT.h"
SoftwareSerial ESP8266(2, 3); // Rx,  Tx
  
long writingTimer = 17; 
long startTime = 0;
long waitTime = 0;

const int sensor=A0; // Assigning analog pin A0 to variable 'sensor'
const int rain_sensor=4; // Assigning analog pin A1 to variable 'sensor'
const int DHT11_Sesnor = 5;
#define DHTTYPE DHT11
DHT dht(DHT11_Sesnor, DHTTYPE);
int humudity_value;
float tempc;  //variable to store temperature in degree Celsius
float vout;  //temporary variable to hold sensor reading
bool rain_status = 0;
unsigned char check_connection=0;
unsigned char times_check=0;
boolean error;

String myAPIkey = "8GX33VU8QE8WXEH9";  //Your Write API Key from Thingsspeak

void setup()
{
  Serial.begin(9600); 
  ESP8266.begin(9600); 
  pinMode(rain_sensor, INPUT);
  dht.begin();
  startTime = millis(); 
  delay(2000);
  Serial.println("Connecting to Wifi");
   while(check_connection==0)
  {
    Serial.print(".");
  ESP8266.print("AT+CWJAP=\"Rahul Jadhav 2.4g\",\"JadhRa@2310\"\r\n");
  ESP8266.setTimeout(5000);
 if(ESP8266.find("WIFI CONNECTED\r\n")==1)
 {
 Serial.println("WIFI CONNECTED");
 break;
 }
 times_check++;
 if(times_check>3) 
 {
  times_check=0;
   Serial.println("Trying to Reconnect..");
  }
  }
}

void loop()
{
  waitTime = millis()-startTime;   
  if (waitTime > (writingTimer*1000)) 
  {
    vout=analogRead(sensor);
    vout=(vout*500)/1023;
    tempc=vout; // Storing value in Degree Celsius
    rain_status = digitalRead(rain_sensor);
    readDTH11_Sesnor();
    writeThingSpeak();
    startTime = millis();   
  }
}

void readDTH11_Sesnor()
{

  // Reading temperature or humidity takes about 250 milliseconds!
  humudity_value = dht.readHumidity();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humudity_value))
  {
    Serial.println(("Failed to read from DHT sensor!"));
    return;
  }
 // Serial.print((" Humidity: "));
  //Serial.print(humudity_value);
 // Serial.print(("%"));

//  Serial.print("\n"); 
 // delay(100); 
}

void writeThingSpeak(void)
{
  startThingSpeakCmd();
  // preparacao da string GET
  String getStr = "GET /update?api_key=";
  getStr += myAPIkey;
  getStr +="&field1=";
  getStr += String(tempc);
  getStr +="&field2=";
  getStr += String((rain_status == 0 ? 1:0));
  getStr +="&field3=";
  getStr += String(humudity_value);
  getStr += "\r\n\r\n";
  GetThingspeakcmd(getStr); 
}

void startThingSpeakCmd(void)
{
  ESP8266.flush();
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com IP address
  cmd += "\",80";
  ESP8266.println(cmd);
  Serial.print("Start Commands: ");
  Serial.println(cmd);

  if(ESP8266.find("Error"))
  {
    Serial.println("AT+CIPSTART error");
    return;
  }
}

String GetThingspeakcmd(String getStr)
{
  String cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ESP8266.println(cmd);
  Serial.println(cmd);

  if(ESP8266.find(">"))
  {
    ESP8266.print(getStr);
    Serial.println(getStr);
    delay(500);
    String messageBody = "";
    while (ESP8266.available()) 
    {
      String line = ESP8266.readStringUntil('\n');
      if (line.length() == 1) 
      { 
        messageBody = ESP8266.readStringUntil('\n');
      }
    }
    Serial.print("MessageBody received: ");
    Serial.println(messageBody);
    return messageBody;
  }
  else
  {
    ESP8266.println("AT+CIPCLOSE");     
    Serial.println("AT+CIPCLOSE"); 
  } 
}
