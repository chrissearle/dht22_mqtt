#include <PubSubClient.h>
#include <ArduinoLog.h>

#include "Configuration.h"
#include "Board.h"
#include "Sensor.h"

void Board::reconnect()
{
    while (!pubSubClient->connected())
    {
        Log.noticeln("Attempting MQTT connection...");

        String clientId = String("DHT-" + String(clientName));

        if (pubSubClient->connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD))
        {
            Log.noticeln("connected");
        }
        else
        {
            Log.warningln("failed, rc=%d try again in 5 seconds", pubSubClient->state());
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void Board::keepAlive()
{
    if (!pubSubClient->connected())
    {
        reconnect();
    }

    pubSubClient->loop();
}

void Board::addSensor(Sensor *sensor)
{
    sensorList[sensorCount] = sensor;
    sensorCount = sensorCount + 1;
}

void Board::publish()
{
    for (int i = 0; i < sensorCount; i++)
    {
        Sensor *sensor = sensorList[i];

        const char *sensorName = sensor->getName();

        publishToTopic(sensorName, "temperature", sensor->getTemperature());
        publishToTopic(sensorName, "humidity", sensor->getHumidity());
    }
}

void Board::publishToTopic(const char *topicName, const char *topicType, float value)
{
  char *publishTopic = (char *)malloc(50);
  
  memset(publishTopic, 0, 50);

  sprintf(publishTopic, "%s/%s/%s", MQTT_TOPIC, topicName, topicType);

#if SENDMQ
    Log.noticeln("Posting");
    char charVal[10];
    dtostrf(value, 4, 4, charVal);
    pubSubClient->publish(publishTopic, charVal);
#endif

    Log.noticeln("%s %F", publishTopic, value);
}
