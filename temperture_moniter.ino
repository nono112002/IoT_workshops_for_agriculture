// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
#include "M5AtomS3.h"
#include <Ambient.h>
#include <WiFi.h>

unsigned int channelId = ;
const char* writeKey = "";
const char* ssid = "";
const char* password = "";

WiFiClient client;
Ambient ambient;

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 32 //M5AtomLite
#define TEMPERATURE_PRECISION 9

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// arrays to hold device addresses
DeviceAddress Sensor1 = { 0x28, 0xF3, 0xA3, 0x71, 0x0F, 0x00, 0x00, 0x30 };
DeviceAddress Sensor2 = { 0x28, 0x4E, 0xE6, 0x72, 0x0F, 0x00, 0x00, 0xCE };
DeviceAddress Sensor3 = { 0x28, 0xB1, 0x74, 0x72, 0x0F, 0x00, 0x00, 0xAC };

// 送信間隔
unsigned long sendInterval = 10 * 1000;  //mSec.

void setup(void) {
  M5.begin();
  // start serial port
  Serial.begin(115200);

  //DeepSleep設定
  esp_sleep_enable_timer_wakeup(sendInterval * 1000);

  //WiFiを切断
  WiFi.disconnect();
  delay(1000);

  // WiFi接続
  WiFi.begin(ssid, password);
  unsigned long startAttemptTime = millis();  // 接続試行の開始時間を記録
  const unsigned long wifiTimeout = 60000;    // タイムアウト時間（1分 = 60000ミリ秒）
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("_");
    // タイムアウトチェック
    if (millis() - startAttemptTime >= wifiTimeout) {
      Serial.println("WiFi connection failed, going to sleep...");
      Serial.println("WiFi connection failed, going to deep sleep...");
      
      // デバイスをDeep Sleepに移行
      esp_deep_sleep_start();
    }
  }

  // Ambient接続処理を関数で呼び出す
  connectToAmbient();

  // start the Dallas Temperature library
  sensors.begin();

  // set the resolution to 9 bit per device
  sensors.setResolution(Sensor1, TEMPERATURE_PRECISION);
  sensors.setResolution(Sensor2, TEMPERATURE_PRECISION);
  sensors.setResolution(Sensor3, TEMPERATURE_PRECISION);

  // request temperatures from the sensors
  sensors.requestTemperatures();

  // print the device information and send data to Ambient
  ambient.set(1, printTemperature(Sensor1));
  ambient.set(2, printTemperature(Sensor2));
  ambient.set(3, printTemperature(Sensor3));
  bool sendResult = ambient.send();
  if (sendResult) {
    Serial.println("Data sent successfully");
  } else {
    Serial.println("Failed to send data");
  }

  // WiFi切断
  WiFi.disconnect();
  delay(1000);
  
  // デバイスをDeep Sleepに移行
  esp_deep_sleep_start();
}

// Ambient接続を行う関数
void connectToAmbient() {
  //Ambientに接続
  if (ambient.begin(channelId, writeKey, &client)) {
    Serial.println("Ambient connected successfully");
  } else {
    Serial.println("Failed to connect Ambient");
  }
}

// function to print the temperature for a device
float printTemperature(DeviceAddress deviceAddress) {
  float tempC = sensors.getTempC(deviceAddress);
  if (tempC == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: Could not read temperature data");
    return 0;
  }
  Serial.print("Temp C: ");
  Serial.print(tempC);
  Serial.print(" Temp F: ");
  Serial.print(DallasTemperature::toFahrenheit(tempC));
  Serial.println();
  return tempC;
}

void loop(void) {
  // このプログラムではloop()は使用されません
}


