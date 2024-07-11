int led = 18;//led

//const int publishQoS = 1;
const int subscribeQoS = 1;

int state1;
int state2;
int state3;
int state4;
int state5;
int state6;
int state7;
int state8;

int device1;
int device2;
int device3;
int device4;
int device5;
int device6;
int speed=0;
int speed_1=0;
int wifi_led; 

const int d1 = 200;
const int d2 = 250;
const int d3 = 300;// eeprom store address
const int d4 = 350;
const int d5 = 400;
const int d6 = 450;
const int d7 = 500;
const int d8 = 550;

const int resetPin = 19;
unsigned long buttonPressStart = 0;
bool reset_button_pressed = false;

const char* ntpServer = "pool.ntp.org";
unsigned long epochTime;
IPAddress localIP;
unsigned char receivedChar;

unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return (0);
  }
  time(&now);
  return now;
}

void intializepins()
{
  pinMode(resetPin, INPUT);

  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
 
}
void blinkled3time()
{
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  delay(1000);
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  delay(1000);
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  delay(1000);
}
