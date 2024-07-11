char softreset_status[200];
char ssid[32];
char password[64];
char status_switch[500];


const char* mqttServer = "mqtt.onwords.in";// mqttt
const int mqttPort = 1883;
const char* mqttUsername = "Nikhil";
const char* mqttPassword = "Nikhil8182";

char hardware_current_Status[300];
char hardware_productid[100];

WiFiServer server(8182);
WiFiClient espClient;
PubSubClient client(espClient);

void paringMode()
{
  WiFi.mode(WIFI_AP);
  String hotspot = "onwords-" + PId; //hotsopt name with pid
  const char* hotspot_name = hotspot.c_str();// hotspot creation
  WiFi.softAP(hotspot_name, "");  // assigning name to hotspot
  server.begin();                 //starting http server
  WiFi.softAPIP();
  Serial.println(WiFi.softAPIP());//print webserver local ip by default 192.168.4.1
  delay(2000);                    //this is mandatory delay
}

void Statusdevice1(int d1)
{
  DynamicJsonDocument sts(6000);
  sts["device1"] = d1;
  sts["client_id"] = "Touch";
  sts["id"] = PId;
  sts["ip"] = localIP.toString();
  sts["time_stamp"] = epochTime;
  serializeJson(sts, status_switch);
  client.publish(topic7, status_switch);
}
void Statusdevice2(int d2)
{
  DynamicJsonDocument sts(6000);
  sts["device2"] = d2;
  sts["client_id"] = "Touch";
  sts["id"] = PId;
  sts["ip"] = localIP.toString();
  sts["time_stamp"] = epochTime;
  serializeJson(sts, status_switch);
  client.publish(topic7, status_switch);
}
void Statusdevice3(int d3)
{
  DynamicJsonDocument sts(6000);
  sts["device3"] = d3;
  sts["client_id"] = "Touch";
  sts["id"] = PId;
  sts["ip"] = localIP.toString();
  sts["time_stamp"] = epochTime;
  serializeJson(sts, status_switch);
  client.publish(topic7, status_switch);
}
void Statusdevice4(int d4)
{
  DynamicJsonDocument sts(6000);
  sts["device4"] = d4;
  sts["client_id"] = "Touch";
  sts["id"] = PId;
  sts["ip"] = localIP.toString();
  sts["time_stamp"] = epochTime;
  serializeJson(sts, status_switch);
  client.publish(topic7, status_switch);
}

void Statusdevice5(int d5, int speed)
{
  DynamicJsonDocument sts(6000);
  sts["device5"] = d5;
  sts["speed"] = speed;
  sts["client_id"] = "Touch";
  sts["id"] = PId;
  sts["ip"] = localIP.toString();
  sts["time_stamp"] = epochTime;
  serializeJson(sts, status_switch);
  client.publish(topic7, status_switch);
}
void Statusdevice6(int d6, int speed_1){
  DynamicJsonDocument sts(6000);
  sts["device6"] = d6;
  sts["speed_1"] = speed_1;
  sts["client_id"] = "Touch";
  sts["id"] = PId;
  sts["ip"] = localIP.toString();
  sts["time_stamp"] = epochTime;
  serializeJson(sts, status_switch);
  client.publish(topic7, status_switch);
}

void currentstatus(int device1, int device2, int device3, int device4,int device5,int device6, int speed,int speed_1,int wifi_led)
{
  EEPROM.put(d1 , device1);
  EEPROM.commit();
  delay(50);
  EEPROM.put(d2 , device2);
  EEPROM.commit();
  delay(50);
  EEPROM.put(d3 , device3);
  EEPROM.commit();
  delay(50);
  EEPROM.put(d4 , device4);
  EEPROM.commit();
  delay(50);
  EEPROM.put(d5 , device5);
  EEPROM.commit();
  delay(50);
  EEPROM.put(d6 , device6);
  EEPROM.commit();
  delay(50);
  EEPROM.put(d7 , speed);
  EEPROM.commit();
  delay(50);
  EEPROM.put(d8 , speed_1);
  EEPROM.commit();
  delay(50);
  
  StaticJsonDocument<2000> doc;
  doc["device1"] = device1;
  doc["device2"] = device2;
  doc["device3"] = device3;
  doc["device4"] = device4;
  doc["device5"] = device5;
  doc["speed"] = speed;
  doc["device6"] = device6;
  doc["speed_1"] = speed_1;
  doc["wifi_led"] = wifi_led;
   
  serializeJson(doc, hardware_current_Status);
  client.publish(topic3, hardware_current_Status, false);
}
void callback(char* topic, byte * payload, unsigned int length)
{
  Serial.print(topic);
  String aa = "";
  for (int i = 0; i < length; i++) {
    aa += (char)payload[i];
  }
  Serial.println(aa);
  DynamicJsonDocument doc(6000);
  DeserializationError error = deserializeJson(doc, aa); // Deserialize the message into the JSON document
  if (error) {
    return;
  }
  int firmware_sts = doc["firmware_sts"];
  if (firmware_sts == 1)
  {
    String firmwareUrl = doc["firmwareUrl"];
    frimware_update(firmwareUrl);
  }

   if (doc.containsKey("wifi_led")) {
    wifi_led = doc["wifi_led"];
    if (wifi_led == 1 )
    {
      digitalWrite(led, HIGH);
    }
    if (wifi_led == 0 )
    {
      digitalWrite(led, LOW);
    }
  }
  
  if (doc.containsKey("device1")) {
    device1 = doc["device1"];
    if (device1 == 1 )
    {
      Serial.println("L1");
       

    }
    if (device1 == 0 )
    {
      Serial.println("L0");
     

    }
  }
  if (doc.containsKey("device2")) {
    device2 = doc["device2"];
    if (device2 == 1 )
    {
      Serial.println("M1");
     
      

    }
    if (device2 == 0 )
    {
      Serial.println("M0");
       

    }

  }

  if (doc.containsKey("device3")) {
    device3 = doc["device3"];
    if (device3 == 1 )
    {
      Serial.println("N1");
     

    }
    if (device3 == 0 )
    {
      Serial.println("N0");
       

    }
  }

   if (doc.containsKey("device4")) {
    device4 = doc["device4"];
    if (device4 == 1 )
    {
      Serial.println("P1");
      

    }
    if (device4 == 0 )
    {
      Serial.println("P0");
       

    }
  }
  if (doc.containsKey("device5")) {
    device5 = doc["device5"];
    if (device5 == 1 )
    {
      Serial.println("F1");
      

    }
    if (device5 == 0)
    {
      Serial.println("F0");
       

    }
  }
  
  if (doc.containsKey("speed")) {
    speed = doc["speed"];
    if (device5 == 1){
    Serial.print("U");
    Serial.println(speed);
     

  }
  }

  if (doc.containsKey("device6")) {
    device6 = doc["device6"];
    if (device6 == 1 )
    {
      Serial.println("G1");
    

    }
    if (device6 == 0)
    {
      Serial.println("G0");
      

    }
  
  if (doc.containsKey("speed_1")) {
    speed_1 = doc["speed_1"];
    if (device6 == 1){
    Serial.print("H");
    Serial.println(speed_1);
  }
  }
  }
  currentstatus(device1, device2, device3, device4,device5,device6,speed,speed_1,wifi_led);
  
  
  
  String response = doc["request"];
  if (response == "getCurrentStatus")
  {
   DynamicJsonDocument docs  (7000);
    docs["device1"] = device1;
    docs["device2"] = device2;
    docs["device3"] = device3;
    docs["device4"] = device4;
    docs["device5"] = device5;
    docs["speed"] = speed;
    docs["device6"] = device6;
    docs["speed_1"] = speed_1;
    docs["wifi_led"] = wifi_led;
    
    
    serializeJson(docs, hardware_current_Status);
    client.publish(topic3,hardware_current_Status, false);
  }
  int soft_resets = doc["reset"];
  if (soft_resets == 1)
  {
    for (int i = 0 ; i < 60; i++)
    {
      EEPROM.write(i, 0);
    }
    EEPROM.end();
    delay(100);
    StaticJsonDocument <2000>soft;
    soft["status"] = true;
    serializeJson(soft, softreset_status);
    client.publish(topic5,softreset_status);
    delay(500);
    ESP.restart();
  }
}
