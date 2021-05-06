#include "DHT.h"
DHT dht;
int analogPin = A0; //ประกาศตัวแปร ให้ analogPin แทนขา analog ขาที่5
int val = 0;
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)");

  dht.setup(2); // data pin 2
}

void loop() {
  //read data from soil
  val = analogRead(analogPin);  
  //read data from environment
  delay(dht.getMinimumSamplingPeriod());
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  float fahrenheit = dht.toFahrenheit(temperature)
  delay(100);
  //transmit data from soil sensor
  //recieve springkle status from STM32
  //transmit data to website (all data from sensor & springkle status)
}
