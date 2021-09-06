#ifndef SENSOR_H
#define SENSOR_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <DHT.h>
#include<ArduinoLog.h>

class Sensor
{
private:
  char *sensorName;
  DHT *dht;

public:
  Sensor(const char *name, byte pin)
  {
    sensorName = (char *)malloc(strlen(name) + 1);
    memset(sensorName, 0, strlen(name) + 1);
    memcpy(sensorName, name, strlen(name));

    Log.noticeln("Initializing sensor %s", sensorName);

    dht = new DHT(pin, DHT22);

    dht->begin();
  };

  char *getName();

  float getTemperature();
  float getHumidity();
};

#endif
