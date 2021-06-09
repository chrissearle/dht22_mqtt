#include "Sensor.h"

char *Sensor::getName()
{
    return sensorName;
}

float Sensor::getHumidity()
{
    return dht->readHumidity();
}

float Sensor::getTemperature()
{
    return dht->readTemperature();
}