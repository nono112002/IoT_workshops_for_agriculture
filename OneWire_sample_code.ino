#include <OneWire.h>
#include <DallasTemperature.h>
#include "M5Atom.h"

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 32
#define TEMPERATURE_PRECISION 9

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// arrays to hold device addresses
DeviceAddress Sensor1 = { 0x28, 0x4C, 0xB6, 0x71, 0x0F, 0x00, 0x00, 0xBD };
// DeviceAddress Sensor2 = { 0x28, 0x4E, 0xE6, 0x72, 0x0F, 0x00, 0x00, 0xCE };
// DeviceAddress Sensor3 = { 0x28, 0xB1, 0x74, 0x72, 0x0F, 0x00, 0x00, 0xAC };

void setup(void) {
  M5.begin();
  Serial.begin(115200);
  // start the Dallas Temperature library
  sensors.begin();

  sensors.setResolution(Sensor1, TEMPERATURE_PRECISION);
  // sensors.setResolution(Sensor2, TEMPERATURE_PRECISION);
  // sensors.setResolution(Sensor3, TEMPERATURE_PRECISION);
}

void loop(void) {
  // request temperatures from the sensors
  sensors.requestTemperatures();

  printTemperature(Sensor1);
  // printTemperature(Sensor2);
  // printTemperature(Sensor3);

  delay(10000); // 10秒（10000ミリ秒）待機
}


float printTemperature(DeviceAddress deviceAddress) {
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.print(tempC);
  Serial.println();
  return tempC;
}
