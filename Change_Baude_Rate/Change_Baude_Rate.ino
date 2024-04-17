#include <SoftwareSerial.h>
#include <stdlib.h>

SoftwareSerial ESP8266(2, 3); // RX, TX
void setup() {
  Serial.begin(115200);  
  ESP8266.begin(115200);  

     ESP8266.print("***VER:");
  delay(2000);
ESP8266.println("AT+RST");
 delay(1000);
}


void loop() {
ESP8266.println("AT+GMR");

delay(3000);
ESP8266.println("AT+CWMODE=3");

delay(3000);
ESP8266.println("AT+UART_DEF=9600,8,1,0,0");  

delay(3000);
Serial.println("Done");
while(1);
   
}
