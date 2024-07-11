#include <WiFi.h>                    // library
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <Update.h>
#include <PubSubClient.h>
#include <time.h>

#include"print.h"
#include"mqtt_topic.h"
#include"initial_function.h"
#include"firmware.h"
#include"mqtt_fun.h"
#include"eeprom_storage.h"
#include"offline_reconnect.h"

//char receivedChar; 

void setup()
{
  Serial.begin(9600);
  EEPROM.begin(EEPROM_SIZE);
  String ssid = readssid();
  String password = readpassword() ;
  p("ssid = " + ssid);
  p("password = " + password);
  PId = readPid();
  p("PId=" + PId);
  delay(1000);
  intializepins();
  Serial.println("hi");
  if (ssid == "")
  {
    blinkled3time();  // will work only one time its a bug
    paringMode();
  }
  else
  {
    WiFi.begin(ssid.c_str(), password.c_str());
    reconnecting_wifi();
    digitalWrite(led, HIGH);
    parinMode = false;
    delay(200);
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);
    mqtt_topics();

    EEPROM.get(d1, state1);
    EEPROM.get(d2, state2);
    EEPROM.get(d3, state3);
    EEPROM.get(d4, state4);
    EEPROM.get(d5, state5); 
    EEPROM.get(d6, state6);
    EEPROM.get(d7, state7);
    EEPROM.get(d8, state8);

    device1 = state1;
    device2 = state2;
    device3 = state3;
    device4 = state4;
    device5 = state5;
    device6 = state6;
    if(state7 == 1 || state7 == 2 || state7 == 3 || state7 == 4 || state7 == 5)
    {
      state7 = 3;
      speed = state7; 
    }
    else{
      state7 = 0;
      speed = state7;
    }
    Serial.println(state8);
    if(state8 == 1 || state8 == 2 || state8 == 3 || state8 == 4 || state8 == 5)
    {
      state8 = 3;
      speed_1 = state8; 
    }
    else{
      state8 = 0;
      speed_1 = state8;
    }
  }
  configTime(0, 0, ntpServer);
}
void loop()
{
  if (parinMode)
  {
   
    clients = server.available(); // need
    if (!clients) { // WIFIMANAGER
      return;
    }
    while (!clients.available()) {
      delay(1);
    }
    delay(2000);
    pair();
  }
  else
  {
    listen_for_hardreset();
  }
  if (!client.connected())
  {  
      reconnect();
  }
  client.loop();
 

  if (Serial.available() > 0) {
    char receivedChar = Serial.read();
  //  Serial.print("Received: ");
   // Serial.println(receivedChar);
      if (receivedChar == 'S') {
        device1 = ! device1;
        currentstatus(device1, device2, device3,device4,device5,device6,speed,speed_1,wifi_led);
        Statusdevice1(device1);
      } 
      
      else if (receivedChar == 'P') {
        device2 = ! device2;
      currentstatus(device1, device2, device3,device4,device5,device6,speed,speed_1,wifi_led);
      Statusdevice2(device2);
      }

      else if (receivedChar == 'R') {
        device3 = ! device3;
        currentstatus(device1, device2, device3,device4,device5,device6,speed,speed_1,wifi_led);
        Statusdevice3(device3);
      }

      else if (receivedChar == 'G') {
        device4 = ! device4;
        currentstatus(device1, device2, device3,device4,device5,device6,speed,speed_1,wifi_led);
        Statusdevice4(device4);
      }

       else if (receivedChar == 'A') {
        device5=1;
        speed=1;
        currentstatus(device1, device2, device3,device4,device5,device6,speed,speed_1,wifi_led);
        Statusdevice5(device5, speed);
      }
      else if (receivedChar == 'B') {
        device5=0;
       speed=0;
        currentstatus(device1, device2, device3,device4,device5,device6,speed,speed_1,wifi_led);
        Statusdevice5(device5, speed);
      }

       else if (receivedChar == 'C') {
        device5=1;
        speed=2;
        currentstatus(device1, device2, device3,device4,device5,device6,speed,speed_1,wifi_led);
        Statusdevice5(device5, speed);
      }

      else if (receivedChar == 'D') {
      device5=1;
      speed=3;
      currentstatus(device1, device2, device3,device4,device5,device6,speed,speed_1,wifi_led);
      Statusdevice5(device5, speed);
      }
      else if (receivedChar == 'E') {
        device5=1;
        speed=4;
        currentstatus(device1, device2, device3,device4,device5,device6,speed,speed_1,wifi_led);
        Statusdevice5(device5, speed);
      }

      else if (receivedChar == 'F') {
      device5=1;
      speed=5;
      currentstatus(device1, device2, device3,device4,device5,device6,speed,speed_1,wifi_led);
      Statusdevice5(device5, speed);

      }


      else if (receivedChar == 'K') {
        device6=1;
       speed_1=1;
       currentstatus(device1, device2, device3,device4,device5,device6,speed,speed_1,wifi_led);
       Statusdevice6(device6, speed_1);
      }
      else if (receivedChar == 'W') {
        device6=0;
       speed_1=0;
        currentstatus(device1, device2, device3,device4,device5,device6,speed,speed_1,wifi_led);
        Statusdevice6(device6, speed_1);
      }

       else if (receivedChar == 'Z') {
        device6=1;
        speed_1=2;
       currentstatus(device1, device2, device3,device4,device5,device6,speed,speed_1,wifi_led);
       Statusdevice6(device6, speed_1);
      }

      else if (receivedChar == 'X') {
        device6=1;
        speed_1=3;
       currentstatus(device1, device2, device3,device4,device5,device6,speed,speed_1,wifi_led);
       Statusdevice6(device6, speed_1);
      }
      else if (receivedChar == 'I') {
        device6=1;
        speed_1=4;
        currentstatus(device1, device2, device3,device4,device5,device6,speed,speed_1,wifi_led);
        Statusdevice6(device6, speed_1);
      }

      else if (receivedChar == 'J') {
        device6=1; 
        speed_1=5;
        currentstatus(device1, device2, device3,device4,device5,device6,speed,speed_1,wifi_led);
        Statusdevice6(device6, speed_1);
      }

     else if (receivedChar == 'Q'){
     Serial.println("hardreset");
      for (int i = 0 ; i < 60  ; i++) {
        EEPROM.write(i, 0);
      }

      EEPROM.end();
      delay(100);
      StaticJsonDocument<2000> pids;
      pids["id"] = PId;
      serializeJson(pids, hardware_productid);
      client.publish(topic4, hardware_productid);
      delay(500);
      ESP.restart();
    }   
  }
  epochTime = getTime();
}
