/*
Project:  Heater Project - Relay part - with MQTT und OTA update
Author:   Thomas Edlinger for www.edistechlab.com
Date:     Created 30.10.2021
Version:  V1.0
IDE:      Arduino IDE 1.8.16
 
Required Board (Tools -> Board -> Boards Manager...)
 - Board: esp8266 by ESP8266 Community   V3.0.2 
 - Board: esp32   by Espressif Systems   V1.0.6
 
Required libraries (sketch -> include library -> manage libraries)
 - PubSubClient by Nick ‘O Leary V2.8.0
 - ArduinoOTA by Juraj Andrassy V1.0.5

Wirering for the Module:
Modul       ESP32       ESP8266
VCC         D4/4        D2/4
GND         GND         GND
DO          D5/5        D1/5
*/

#include <PubSubClient.h>
#include <ArduinoOTA.h>
//#include <WiFi.h>  // ESP32 only

#define wifi_ssid "Your_SSID"
#define wifi_password "Your_Password"
#define mqtt_server "MQTT_Server_IP"
#define mqtt_user "MQTT_username"         
#define mqtt_password "MQTT_PW"
#define ESPHostname "Heater_Relay"
String clientId = "Heater_Relay-"; 

#define inTopic "HeaterRelayIn"
#define outTopic "HeaterRelayOut"
#define state_topic "HeaterRelayState"

const int heaterPin = 5;

WiFiClient espClient;  
PubSubClient client(espClient);  

void setup() {
  Serial.begin(115200);
  setup_wifi();
  ArduinoOTA.setHostname(ESPHostname);
  // ArduinoOTA.setPassword("admin");
  ArduinoOTA.begin();
  client.setServer(mqtt_server, 1883); 
  client.setCallback(callback); 
  pinMode(heaterPin, OUTPUT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  ArduinoOTA.handle(); 
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}  

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if (String(topic) == inTopic) {
    if(messageTemp == "ON"){
      Serial.print("Changing output to ON\n");
      digitalWrite(heaterPin, HIGH);
      client.publish(state_topic, "ON");
      delay(200);
    }
    else if(messageTemp == "OFF"){
      Serial.print("Changing output to OFF\n");
      digitalWrite(heaterPin, LOW);
      client.publish(state_topic, "OFF"); 
      delay(200);
    }
  }
}
      
 void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(outTopic, ESPHostname);
      // ... and resubscribe
      client.subscribe(inTopic);
    } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(5000);
      }
   }
}
