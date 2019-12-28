/*
 * Youtube subscriber counter.
 * 
 * All required parts can be found here: https://kit.co/colorscream/live-subscriber-counter
 * Assembly video can be found here: https://www.youtube.com/watch?v=feuPataYqzk
 */ 

// Libraries
#include <Adafruit_LEDBackpack.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <YoutubeApi.h>
#include <ArduinoJson.h> // Downgrade this plugin to version 5.13.5

// Wifi credentials
char ssid[] = "Network ID";
char pass[] = "Network password";

// Youtube API credentials
char api_key[] = "Your google API key"; // Get this here: https://developers.google.com/youtube/v3/getting-started
char channel_id[] = "You channel's ID"; // Get this here: https://support.google.com/youtube/answer/3250431?hl=en

// Instantiate
Adafruit_7segment matrix = Adafruit_7segment();
WiFiClientSecure client;
YoutubeApi api(api_key, client);

void setup() {
  client.setInsecure();
  
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  
  matrix.begin(0x70);
  matrix.setBrightness(99);
}

void loop() {
  /* If the youtube data is available set the brightness to full, 
   * write the subcount on the screen and wait 10 minutes 
   */
  if (api.getChannelStatistics(channel_id)) {
    matrix.setBrightness(99);
    matrix.print(api.channelStats.subscriberCount, DEC);
    matrix.writeDisplay();
    delay(600000);
  } 
  /* If the youtube data is not available dimm the screen 
   * display hyphens on the screen and retry in 10 seconds.  
   */
  else {
    matrix.setBrightness(15);
    matrix.writeDigitRaw(0, B01000000);
    matrix.writeDigitRaw(1, B01000000);
    matrix.writeDigitRaw(3, B01000000);
    matrix.writeDigitRaw(4, B01000000);
    matrix.writeDisplay();
    delay(10000);
  }
}
