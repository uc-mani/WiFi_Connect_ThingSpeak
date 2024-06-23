#include <Arduino.h>
#include "WiFi.h"
#include "ThingSpeak.h"
#include <../../credentials.h>

#define TIMEOUT_MS 20000
#define LED_PIN 22

WiFiClient client;
int counter = 0;

void ConnectToWifi(){
  Serial.print("\nConnecting to WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_NETWORK,WIFI_PASSWORD); //these two params are saved in secure file
  unsigned long startAttemptTime=millis();//return uptime of esp32
  while(WiFi.status() != WL_CONNECTED && (millis()-startAttemptTime < TIMEOUT_MS)){
    Serial.print(".");
    delay(100);
  }
  if(WiFi.status() != WL_CONNECTED){
    Serial.println(" Failed!");
  }else{
    Serial.print("Connected! IP is: ");
    Serial.println(WiFi.localIP());
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  ConnectToWifi();
  ThingSpeak.begin(client);
}

void loop() {
  digitalWrite(LED_PIN, WiFi.status() == WL_CONNECTED); // turn LED ON when wifi is connected
  counter++;
  ThingSpeak.setField(1, counter);
  ThingSpeak.setField(2, WiFi.RSSI());
  ThingSpeak.writeFields(CHANNEL_ID, CHANNEL_API_KEY); //these two params are saved in secure file

  delay(15000);
}