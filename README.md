# esp8266-wifi-direct-examples
Server and client example to wifi control a Adafruit PCA9685 servo controller

This will allow you to take advantage of the esp8266's WIFI direct feature, aka p2p feature.  A client esp8266 connects to a server esp8266 with no access point inbetween, and establishes a TCP socket connection.  Analog data is read from potentiometers wired to a 4051 multiplexer to the esp's single analog input.  Data is sent as a casted array to the server.  The speed is very fast, possibly up to HD video quality streaming.

IMPORTANT NOTES:

There is little information on the internet about how to p2p the esp8266.  The majority of the issue is because the wifi mode on the client must be set to Wifi_STA and the server must be set to Wifi_AP.  If Wifi_AP_STA or Wifi_STA is used for the server, the client will connect and send some little data, and then quickly disconnect.

client.setNoDelay(1) must be used or transmission will be slow due to the Nagle algorithm.

Any calls to server.available() will disconnect the current client.  For multiple clients a array and queue can be used to keep previous connections alive (google examples).
