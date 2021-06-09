#ifndef BOARD_H
#define BOARD_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <PubSubClient.h>

#include "Configuration.h"
#include "Sensor.h"

class Board
{
private:
  PubSubClient *pubSubClient;
  Sensor *sensorList[5];
  byte sensorCount = 0;
  char *clientName;

  void reconnect();
  void publishToTopic(const char *topicName, const char *topicType, float value);

public:
  Board(const char *name, Client &client)
  {
    clientName = (char *)malloc(strlen(name) + 1);
    memset(clientName, 0, strlen(name) + 1);
    memcpy(clientName, name, strlen(name));
    Serial.println(clientName);

    pubSubClient = new PubSubClient(client);
    pubSubClient->setServer(MQTT_SERVER, 1883);
  }

  void addSensor(Sensor *sensor);

  void publish();

  void keepAlive();
};

#endif
