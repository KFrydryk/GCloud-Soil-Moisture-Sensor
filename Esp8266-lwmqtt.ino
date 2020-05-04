/******************************************************************************
 * Copyright 2018 Google
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/
#include <CloudIoTCore.h>
#include "esp8266_mqtt.h"

#include <ArduinoJson.h>
#include "movingAvg.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif


StaticJsonDocument<200> JsonDoc;
movingAvg smoothSensor(100);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  setupCloudIoT(); // Creates globals for MQTT
  pinMode(LED_BUILTIN, OUTPUT);
  smoothSensor.begin();
}


char JsonSend[100];
unsigned long lastMillis = 0;

void loop()
{
  mqtt->loop();
  smoothSensor.reading(analogRead(A0));
  delay(10); // <- fixes some issues with WiFi stability
  Serial.println(analogRead(A0));
  if (!mqttClient->connected())
  {
    ESP.wdtDisable();
    connect();
    ESP.wdtEnable(0);
  }

  // TODO: Replace with your code here
  if (millis() - lastMillis > 60000)
  {
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);
    pinMode(4, INPUT);
    JsonDoc["Light"] = 100*(1-digitalRead(4));
    JsonDoc["id"] = 1;
    JsonDoc["time"] = time(nullptr);
    JsonDoc["Humidity"] = map((1023-smoothSensor.getAvg()), 0, 1024, 0, 100);
    serializeJsonPretty(JsonDoc, JsonSend);
    lastMillis = millis();
 
    Serial.println(JsonSend);
    publishTelemetry(JsonSend);
  }
}
