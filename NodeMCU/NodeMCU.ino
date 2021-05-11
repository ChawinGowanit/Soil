#include "DHT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
const char* ssid = "Chalesa-2.4G";
const char* password = "wanee1234";
const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "f436ea82-7c41-41f8-af15-755514f97338";
const char* mqtt_username = "jQKmAd9aQUemRryJtH2vZ5wHFrkdmjja";
const char* mqtt_password = "VJuXM26zViTJ#4yoqlAuDG3si6t!WfuX";
WiFiClient espClient;
PubSubClient client(espClient);
#define DHTPIN 2
#define DHTTYPE DHT11 
char msg[1000];
long lastMag = 0;
DHT dht(DHTPIN, DHTTYPE);

int analogPin = A0;
int Moisture = 0;
char s ;

void reconnect() {
    while (!client.connected()) {
      Serial.print("Attempting MQTT connection…");
      if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
        Serial.println("connected");
      }
      else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println("try again in 5 seconds");
        delay(5000);
      }
    }
  }
  
void setup() {
  Serial.begin(115200);
  dht.begin(); //pin D4

  WiFi.begin(ssid,password);
  while(WiFi.status()!= WL_CONNECTED){
    delay(250);
    Serial.print(".");
  }
  Serial.print("WiFi connect");
  client.setServer(mqtt_server,mqtt_port);
}

void loop() {
  //read data from soil
  Moisture = analogRead(analogPin);  
  //read data from environment
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  float fahrenheit = dht.readTemperature(true);
  //transmit data from soil sensor
  Serial.println(Moisture);
  //recieve springkle status from STM32
   s = Serial.read();
   Serial.println(s);

  //transmit data to website (all data from sensor & springkle status)
  if(!client.connected()){
    reconnect();
  }
  client.loop();
  String data = "{\"data\": {\"Moisture\":" + String(Moisture) + "}}";
  data.toCharArray(msg,(data.length()+1));
  client.publish("@shadow/data/update",msg);
   data = "{\"data\": {\"humidity\":" + String(humidity) + "}}";
  data.toCharArray(msg,(data.length()+1));
  client.publish("@shadow/data/update",msg);
   data = "{\"data\": {\"temperature\":" + String(temperature) + "}}";
  data.toCharArray(msg,(data.length()+1));
  client.publish("@shadow/data/update",msg);
   data = "{\"data\": {\"fahrenheit\":" + String(fahrenheit) + "}}";
  data.toCharArray(msg,(data.length()+1));
  client.publish("@shadow/data/update",msg);
  data = "{\"data\": {\"s\":" + String(s) + "}}";
  data.toCharArray(msg,(data.length()+1));
  client.publish("@shadow/data/update",msg);
  delay(1000);

}
