
#include <ESP8266WiFi.h>

#define Selector_0 16
#define Selector_1 5
#define Selector_2 4
#define Enable_0 14
#define Enable_1 12

#define pot_limit 315
#define timeout 10


IPAddress serverIP(192, 168, 4, 1);


//for ledcWrite
//RANGE OF JX 2060 IS 1800 TO 7800
//RANGE OF MG90S IS 1750 TO 7550

//for adafruit PWM
//RANGE OF MG90S IS 100 TO 500
//RANGE OF JX60 IS 110 TO 460


uint16_t pot_array[8];

void select_channel(int);
WiFiClient client;


void setup() {
  
  pinMode(Selector_0, OUTPUT);
  pinMode(Selector_1, OUTPUT);
  pinMode(Selector_2, OUTPUT);
  pinMode(Enable_0, OUTPUT);
  pinMode(Enable_1, OUTPUT);

  Serial.begin(115200);

  client.setNoDelay(1);

  const char *ssid = "espServer";
  const char *password = "11111111";

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  while ( WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(WiFi.status());
  }

  Serial.println("WIFI connected");
  Serial.println(WiFi.localIP());  // get interface IP address mask

  Serial.println(client.connect(serverIP, 5000));
}


void loop() {

  if (client.connected())
  {
    Serial.println("Connection success!");
  }
  else {
    Serial.println("oops");
    client.connect(serverIP, 5000);
    delay(50);
  }

  for (;;)
  {    
    for (int channel = 0; channel < 8; channel++)
    {
      select_channel(channel);
      pot_array[channel] = analogRead(0);

      if (pot_array[channel] < pot_limit) {
        pot_array[channel] = pot_limit;    
      }     
      
      yield();
      
    }
    
    client.write_P((const char*)pot_array, sizeof(pot_array));
    delay(timeout);
    

    if(!client.connected())
    {
      Serial.println("connection reset");
      client.connect(serverIP, 5000);
      break;
    }
    
   // yield();
  }

}
void select_channel(int channel_num)
{
  if (channel_num < 8)
  {
    digitalWrite(Enable_0, 0);
    digitalWrite(Enable_1, 1);
  }
  else {
    digitalWrite(Enable_0, 1);
    digitalWrite(Enable_1, 0);
  }

  switch (channel_num) {

    case 0:
      digitalWrite(Selector_0, 0);
      digitalWrite(Selector_1, 0);
      digitalWrite(Selector_2, 0);
      break;

    case 1:
      digitalWrite(Selector_0, 1);
      digitalWrite(Selector_1, 0);
      digitalWrite(Selector_2, 0);
      break;


    case 2:
      digitalWrite(Selector_0, 0);
      digitalWrite(Selector_1, 1);
      digitalWrite(Selector_2, 0);
      break;


    case 3:
      digitalWrite(Selector_0, 1);
      digitalWrite(Selector_1, 1);
      digitalWrite(Selector_2, 0);
      break;


    case 4:
      digitalWrite(Selector_0, 0);
      digitalWrite(Selector_1, 0);
      digitalWrite(Selector_2, 1);
      break;


    case 5:
      digitalWrite(Selector_0, 1);
      digitalWrite(Selector_1, 0);
      digitalWrite(Selector_2, 1);
      break;


    case 6:
      digitalWrite(Selector_0, 0);
      digitalWrite(Selector_1, 1);
      digitalWrite(Selector_2, 1);
      break;


    case 7:
      digitalWrite(Selector_0, 1);
      digitalWrite(Selector_1, 1);
      digitalWrite(Selector_2, 1);
      break;
  }
}
