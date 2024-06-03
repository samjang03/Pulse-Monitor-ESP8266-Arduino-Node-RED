#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "secrets.h"
#include "uMQTTBroker.h"

uMQTTBroker myBroker;

const int PULSE_MIN = 60; // Мінімальне значення пульсу (ударів на хвилину)
const int PULSE_MAX = 90; // Максимальне значення пульсу (ударів на хвилину)

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password

const char* mqtt_server = "91.121.93.94";

WiFiClient espClient;
PubSubClient client_mqtt(espClient);

int bpm = 0;
bool notifications = true;

unsigned long lastPulseSampleTime = 0; // Час останнього зчитування пульсу
const int PULSE_SAMPLE_PERIOD = 2000; // Період вибірки пульсу в мілісекундах

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SECRET_SSID);
  WiFi.begin(SECRET_SSID, SECRET_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client_mqtt.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client_mqtt.connect("NodeMCUClient")) {
      Serial.println("connected");  
      client_mqtt.subscribe("esp8266/notifications");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client_mqtt.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(String topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String messageTemp;
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    messageTemp += (char)payload[i];
  }
  Serial.println();

  if(topic=="esp8266/notifications"){
    Serial.print("Notifications are ");
    if(messageTemp == "true"){
      notifications = true;
      Serial.println("ON");
    }
    else if(messageTemp == "false"){
      notifications = false;
      Serial.println("OFF");
    }
  }
}

void setup() {
  Serial.begin(9600);

  setup_wifi();

  Serial.println("Starting MQTT broker");
  client_mqtt.setServer(mqtt_server, 1883);
  client_mqtt.setCallback(callback);
}

void loop() {

  if (!client_mqtt.connected()) {
    reconnect();
  }
  client_mqtt.loop();
  
  if (millis() - lastPulseSampleTime >= PULSE_SAMPLE_PERIOD) {

    lastPulseSampleTime = millis();


    if (Serial.available() > 0) {
      String receivedData = Serial.readStringUntil('\n');

      // Перетворення рядка на число
      bpm = receivedData.toInt();

      if (bpm <= PULSE_MIN || bpm >= PULSE_MAX) {
        if (notifications) {
          client_mqtt.publish("esp8266/pulse_alarm", String(bpm).c_str());
        }
        
      }
  
      // Виведення отриманого значення пульсу в серійний монітор
      Serial.print("Received Pulse: ");
      Serial.println(bpm);
    }

    client_mqtt.publish("esp8266/pulse", String(bpm).c_str());
  }
}
