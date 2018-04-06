#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define LOWFREQ 110
#define HIFREQ 460

//for ledcWrite
//RANGE OF JX 2060 IS 1800 TO 7800
//RANGE OF MG90S IS 1750 TO 7550

//for adafruit PWM
//RANGE OF MG90S IS 100 TO 500
//RANGE OF JX60 IS 110 TO 460

const char* ssid = "espServer";
const char* password = "11111111";
uint8_t readin[16];
uint16_t pot_array[8];

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
uint8_t servonum = 0; // -1 correction

int middle;

WiFiServer server(5000);
WiFiClient client;


void setup() {
  Serial.begin(115200);
  Wire.begin(5,4);
  
  delay(10);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  server.begin();
  server.setNoDelay(1);
  IPAddress ServerIP = WiFi.softAPIP(); // Obtain the IP of the Server
  Serial.print("Server IP is: "); // Print the IP to the monitor window
  Serial.println(ServerIP);

  middle = LOWFREQ + ((HIFREQ - LOWFREQ) / 2);
  pwm.begin();
  pwm.setPWMFreq(50);
}

void loop() {

  // Check if a client has connected

  if (!client.connected()) {
    //Serial.println("no client yet");
    client = server.available();
    return;
  }
  Serial.println("GOT CLIENT!!!!!!!");
  client.setNoDelay(1);

  for (;;)
  {

    while (client.available())
    {


      client.read((uint8_t*)pot_array, sizeof(pot_array));

      for(int x = 0; x < 8; x++)
      {
          pot_array[x] = map(pot_array[x], 315, 1024, LOWFREQ, HIFREQ); //10bit mapping

          Serial.print(pot_array[x]);
          Serial.print(',');
          pwm.setPWM(x, 0, pot_array[x]);
      }
      Serial.println(" ");


      yield();

    }


    if (!client.connected())
    {
      Serial.println("client disconnected");
      //close client and reconnect
      client.flush();
      client.stop();
      break;
    }
  }
}


